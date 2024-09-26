// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include <boost/utility/string_view.hpp>

using namespace Ishiko;

NetworkConnectionsManager::ManagedSocket::ManagedSocket(NetworkConnectionsManager& manager, TCPClientSocket&& socket)
    : m_manager{manager}, m_socket{std::move(socket)}
{
}

int NetworkConnectionsManager::ManagedSocket::read(ByteBuffer& buffer, size_t count, Error& error)
{
    // TODO: if erro need to add socket to select read
    return m_socket.read(buffer, count, error);
}

int NetworkConnectionsManager::ManagedSocket::read(char* buffer, int count, Error& error)
{
    // TODO: if erro need to add socket to select read
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        // TODO: obviously incorrect overwrites other sockets
        FD_ZERO(&m_manager.m_read_ready);
        FD_ZERO(&m_manager.m_write_ready);
        FD_ZERO(&m_manager.m_exception);
        FD_SET(m_socket.m_socket, &m_manager.m_read_ready);
    }
    return n;
}

void NetworkConnectionsManager::ManagedSocket::write(const char* buffer, int count, Error& error)
{
    // TODO: if erro need to add socket to select write
    m_socket.write(buffer, count, error);
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

    FD_ZERO(&m_read_ready);
    FD_ZERO(&m_write_ready);
    FD_ZERO(&m_exception);
}

void NetworkConnectionsManager::connect(IPv4Address address, Port port, ConnectionCallbacks& callbacks, Error& error)
{
    // TODO: this should all be asynchronous
    // TODO: proper error handling
    // TODO: needs to be marked non-blocking
    TCPClientSocket socket(error);
    if (error)
    {
        return;
    }

    socket.connect(address, port, error);
    if (error)
    {
        int last_error = WSAGetLastError();
        if (last_error == WSAEWOULDBLOCK)
        {
            // TODO
            int errrr = 0;
            FD_SET(socket.m_socket, &m_write_ready);
            FD_SET(socket.m_socket, &m_exception);
        }
    }

    // TODO: what if no error? Then we need to make sure is called.

    m_managed_sockets.emplace_back(*this, std::move(socket));
    m_callbacks.emplace_back(&callbacks);
}

void NetworkConnectionsManager::run()
{
    // TODO: all this should be doing is select and notify the sockets of any events relevant to them
    // TODO: can't assume the socket index is 0
    // TODO: need to use select and iterate over all ready sockets
    ManagedSocket& socket = m_managed_sockets[0];
    ConnectionCallbacks& callbacks = *m_callbacks[0];

    struct timeval stTimeOut;
    stTimeOut.tv_sec = 1;
    stTimeOut.tv_usec = 0;
    int ret = select(-1, &m_read_ready, &m_write_ready, &m_exception, &stTimeOut);

    // TODO: check for ret error
    if (FD_ISSET(socket.m_socket.m_socket, &m_write_ready))
    {
        // TODO: can't assume the socket index is 0
        callbacks.onConnectionEstablished(m_managed_sockets[0]);
    }
    if (FD_ISSET(socket.m_socket.m_socket, &m_read_ready))
    {
        // TODO: can't assume the socket index is 0
        callbacks.onReadReady();
    }
}
