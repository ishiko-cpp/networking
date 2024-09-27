// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include "NetworkingErrorCategory.hpp"
#include <boost/utility/string_view.hpp>

using namespace Ishiko;

NetworkConnectionsManager::ManagedSocket::ManagedSocket(NetworkConnectionsManager& manager, TCPClientSocket&& socket,
    ConnectionCallbacks& callbacks)
    : m_manager{manager}, m_socket{std::move(socket)}, m_callbacks{callbacks}
{
}

int NetworkConnectionsManager::ManagedSocket::read(ByteBuffer& buffer, size_t count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            m_manager.setWaitingForRead(m_socket.nativeHandle());
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
            m_manager.setWaitingForRead(m_socket.nativeHandle());
        }
    }
    return n;
}

void NetworkConnectionsManager::ManagedSocket::write(const char* buffer, int count, Error& error)
{
    m_socket.write(buffer, count, error);
    if (error.code() == NetworkingErrorCategory::Value::would_block)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            m_manager.setWaitingForWrite(m_socket.nativeHandle());
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
            setWaitingForWrite(socket.nativeHandle());
            setWaitingForException(socket.nativeHandle());
        }
    }

    // TODO: what if no error? Then we need to make sure callback is called is called.

    m_managed_sockets.emplace_back(*this, std::move(socket), callbacks);
}

void NetworkConnectionsManager::run()
{
    // TODO: all this should be doing is select and notify the sockets of any events relevant to them
    // TODO: can't assume the socket index is 0
    // TODO: need to use select and iterate over all ready sockets
    ManagedSocket& socket = m_managed_sockets[0];

    fd_set fd_read_ready;
    FD_ZERO(&fd_read_ready);
    for (const NativeSocketHandle& socket : m_waiting_for_read)
    {
        FD_SET(socket, &fd_read_ready);
    }

    fd_set fd_write_ready;
    FD_ZERO(&fd_write_ready);
    for (const NativeSocketHandle& socket : m_waiting_for_write)
    {
        FD_SET(socket, &fd_write_ready);
    }

    fd_set fd_exception;
    FD_ZERO(&fd_exception);
    for (const NativeSocketHandle& socket : m_waiting_for_exception)
    {
        FD_SET(socket, &fd_exception);
    }

    struct timeval stTimeOut;
    stTimeOut.tv_sec = 1;
    stTimeOut.tv_usec = 0;
    int ret = select(-1, &fd_read_ready, &fd_write_ready, &fd_exception, &stTimeOut);

    // TODO: check for ret error
    if (FD_ISSET(socket.m_socket.nativeHandle(), &fd_write_ready))
    {
        m_waiting_for_write.erase(socket.m_socket.nativeHandle());
        // TODO: can't assume the socket index is 0
        socket.m_callbacks.onConnectionEstablished(m_managed_sockets[0]);

    }
    if (FD_ISSET(socket.m_socket.nativeHandle(), &fd_read_ready))
    {
        m_waiting_for_read.erase(socket.m_socket.nativeHandle());
        // TODO: can't assume the socket index is 0
        socket.m_callbacks.onReadReady();
    }
}

void NetworkConnectionsManager::setWaitingForRead(NativeSocketHandle socket)
{
    m_waiting_for_read.insert(socket);
}

void NetworkConnectionsManager::setWaitingForWrite(NativeSocketHandle socket)
{
    m_waiting_for_write.insert(socket);
}

void NetworkConnectionsManager::setWaitingForException(NativeSocketHandle socket)
{
    m_waiting_for_exception.insert(socket);
}
