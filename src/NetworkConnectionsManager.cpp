// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include "NetworkingErrorCategory.hpp"
#include <boost/utility/string_view.hpp>

using namespace Ishiko;

NetworkConnectionsManager::ManagedSocket::ManagedSocket(NetworkConnectionsManager& manager, TCPClientSocket&& socket,
    ConnectionCallbacks& callbacks)
    : m_manager{manager}, m_socket{std::move(socket)}, m_callbacks{callbacks}, m_state{State::waiting_for_connection}
{
}

int NetworkConnectionsManager::ManagedSocket::read(ByteBuffer& buffer, size_t count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_manager.setWaitingForRead(m_socket.nativeHandle(), *this);
        }
    }
    return n;
}

int NetworkConnectionsManager::ManagedSocket::read(char* buffer, int count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_manager.setWaitingForRead(m_socket.nativeHandle(), *this);
        }
    }
    return n;
}

void NetworkConnectionsManager::ManagedSocket::write(const char* buffer, int count, Error& error)
{
    m_socket.write(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_write;
            m_manager.setWaitingForWrite(m_socket.nativeHandle(), *this);
        }
    }
}

void NetworkConnectionsManager::ManagedSocket::shutdown(Error& error)
{
    m_socket.shutdown(error);
}

void NetworkConnectionsManager::ManagedSocket::close()
{
    // TODO: this needs to remove the managed socket from the NetworkConnectionManager
    m_socket.close();
}

void NetworkConnectionsManager::ManagedSocket::callback()
{
    switch (m_state)
    {
    case State::waiting_for_connection:
        m_callbacks.onConnectionEstablished(*this);
        break;

    case State::waiting_for_read:
        m_callbacks.onReadReady();
        break;

    case State::waiting_for_write:
        m_callbacks.onWriteReady();
        break;
    }
}

NetworkConnectionsManager::NetworkConnectionsManager()
{
    // TODO: just to avoid reallocations however even that isn't working well
    m_managed_sockets.reserve(100);
}

void NetworkConnectionsManager::connect(IPv4Address address, Port port, ConnectionCallbacks& callbacks, Error& error)
{
    // TODO: this should all be asynchronous
    // TODO: proper error handling
    TCPClientSocket socket(SocketOption::non_blocking, error);
    if (error)
    {
        return;
    }

    socket.connect(address, port, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            NativeSocketHandle handle = socket.nativeHandle();
            m_managed_sockets.emplace_back(*this, std::move(socket), callbacks);
            setWaitingForWrite(handle, m_managed_sockets.back());
            setWaitingForException(handle, m_managed_sockets.back());
        }
    }

    // TODO: what if no error? Then we need to make sure callback is called is called.

    //m_managed_sockets.emplace_back(*this, std::move(socket), callbacks);
}

void NetworkConnectionsManager::run()
{
    // TODO: proper way to terminate
    size_t hack = 10;
    while (--hack)
    {
        fd_set fd_read_ready;
        FD_ZERO(&fd_read_ready);
        for (const std::pair<NativeSocketHandle, ManagedSocket*>& socket : m_waiting_for_read)
        {
            FD_SET(socket.first, &fd_read_ready);
        }

        fd_set fd_write_ready;
        FD_ZERO(&fd_write_ready);
        for (const std::pair<NativeSocketHandle, ManagedSocket*>& socket : m_waiting_for_write)
        {
            FD_SET(socket.first, &fd_write_ready);
        }

        fd_set fd_exception;
        FD_ZERO(&fd_exception);
        for (const std::pair<NativeSocketHandle, ManagedSocket*>& socket : m_waiting_for_exception)
        {
            FD_SET(socket.first, &fd_exception);
        }

        // TODO: make this configurable?
        struct timeval stTimeOut;
        stTimeOut.tv_sec = 1;
        stTimeOut.tv_usec = 0;
        int ret = select(-1, &fd_read_ready, &fd_write_ready, &fd_exception, &stTimeOut);
        // TODO: check for ret error

        for (std::map<NativeSocketHandle, ManagedSocket*>::iterator it = m_waiting_for_write.begin(); it != m_waiting_for_write.end();)
        {
            if (FD_ISSET(it->first, &fd_write_ready))
            {
                it->second->callback();
                it = m_waiting_for_write.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::map<NativeSocketHandle, ManagedSocket*>::iterator it = m_waiting_for_read.begin(); it != m_waiting_for_read.end();)
        {
            if (FD_ISSET(it->first, &fd_read_ready))
            {
                it->second->callback();
                it = m_waiting_for_read.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::map<NativeSocketHandle, ManagedSocket*>::iterator it = m_waiting_for_exception.begin(); it != m_waiting_for_exception.end();)
        {
            if (FD_ISSET(it->first, &fd_exception))
            {
                it->second->callback();
                it = m_waiting_for_exception.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void NetworkConnectionsManager::setWaitingForRead(NativeSocketHandle socket, ManagedSocket& callbacks)
{
    m_waiting_for_read[socket] = &callbacks;
}

void NetworkConnectionsManager::setWaitingForWrite(NativeSocketHandle socket, ManagedSocket& callbacks)
{
    m_waiting_for_write[socket] = &callbacks;
}

void NetworkConnectionsManager::setWaitingForException(NativeSocketHandle socket, ManagedSocket& callbacks)
{
    m_waiting_for_exception[socket] = &callbacks;
}
