// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include "NetworkingErrorCategory.hpp"
#include <boost/utility/string_view.hpp>

using namespace Ishiko;

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
            // TODO: improve this, expecially the crap with .back()
            NativeSocketHandle handle = socket.nativeHandle();
            m_managed_sockets.emplace_back(m_shared_state, std::move(socket), callbacks);
            m_shared_state.setWaitingForWrite(&m_managed_sockets.back());
            m_shared_state.setWaitingForException(&m_managed_sockets.back());
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
        for (ManagedSocketImpl* managed_socket : m_shared_state.m_waiting_for_read)
        {
            FD_SET(managed_socket->socket().nativeHandle(), &fd_read_ready);
        }

        fd_set fd_write_ready;
        FD_ZERO(&fd_write_ready);
        for (ManagedSocketImpl* managed_socket : m_shared_state.m_waiting_for_write)
        {
            FD_SET(managed_socket->socket().nativeHandle(), &fd_write_ready);
        }

        fd_set fd_exception;
        FD_ZERO(&fd_exception);
        for (ManagedSocketImpl* managed_socket : m_shared_state.m_waiting_for_exception)
        {
            FD_SET(managed_socket->socket().nativeHandle(), &fd_exception);
        }

        // TODO: make this configurable?
        struct timeval stTimeOut;
        stTimeOut.tv_sec = 1;
        stTimeOut.tv_usec = 0;
        int ret = select(-1, &fd_read_ready, &fd_write_ready, &fd_exception, &stTimeOut);
        // TODO: check for ret error

        for (std::set<ManagedSocketImpl*>::iterator it = m_shared_state.m_waiting_for_write.begin(); it != m_shared_state.m_waiting_for_write.end();)
        {
            if (FD_ISSET((*it)->socket().nativeHandle(), &fd_write_ready))
            {
                (*it)->callback();
                it = m_shared_state.m_waiting_for_write.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<ManagedSocketImpl*>::iterator it = m_shared_state.m_waiting_for_read.begin(); it != m_shared_state.m_waiting_for_read.end();)
        {
            if (FD_ISSET((*it)->socket().nativeHandle(), &fd_read_ready))
            {
                (*it)->callback();
                it = m_shared_state.m_waiting_for_read.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<ManagedSocketImpl*>::iterator it = m_shared_state.m_waiting_for_exception.begin(); it != m_shared_state.m_waiting_for_exception.end();)
        {
            if (FD_ISSET((*it)->socket().nativeHandle(), &fd_exception))
            {
                (*it)->callback();
                it = m_shared_state.m_waiting_for_exception.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void NetworkConnectionsManager::SharedState::setWaitingForRead(ManagedSocketImpl* managed_socket)
{
    m_waiting_for_read.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForWrite(ManagedSocketImpl* managed_socket)
{
    m_waiting_for_write.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForException(ManagedSocketImpl* managed_socket)
{
    m_waiting_for_exception.insert(managed_socket);
}

NetworkConnectionsManager::ManagedSocketImpl::ManagedSocketImpl(SharedState& shared_state,
    TCPClientSocket&& socket, ConnectionCallbacks& callbacks)
    : m_shared_state{shared_state}, m_socket{std::move(socket)}, m_callbacks{callbacks},
    m_state{State::waiting_for_connection}
{
}

int NetworkConnectionsManager::ManagedSocketImpl::read(ByteBuffer& buffer, size_t count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_shared_state.setWaitingForRead(this);
        }
    }
    return n;
}

int NetworkConnectionsManager::ManagedSocketImpl::read(char* buffer, int count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_shared_state.setWaitingForRead(this);
        }
    }
    return n;
}

void NetworkConnectionsManager::ManagedSocketImpl::write(const char* buffer, int count, Error& error)
{
    m_socket.write(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_write;
            m_shared_state.setWaitingForWrite(this);
        }
    }
}

void NetworkConnectionsManager::ManagedSocketImpl::shutdown(Error& error)
{
    m_socket.shutdown(error);
}

void NetworkConnectionsManager::ManagedSocketImpl::close()
{
    // TODO: this needs to remove the managed socket from the NetworkConnectionManager
    m_socket.close();
}

void NetworkConnectionsManager::ManagedSocketImpl::callback()
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

TCPClientSocket& NetworkConnectionsManager::ManagedSocketImpl::socket()
{
    return m_socket;
}
