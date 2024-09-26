// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include <boost/utility/string_view.hpp>

using namespace Ishiko;

NetworkConnectionsManager::ManagedSocket::ManagedSocket(TCPClientSocket&& socket)
    : m_socket{std::move(socket)}
{

}

int NetworkConnectionsManager::ManagedSocket::read(ByteBuffer& buffer, size_t count, Error& error)
{
    return m_socket.read(buffer, count, error);
}

int NetworkConnectionsManager::ManagedSocket::read(char* buffer, int count, Error& error)
{
    return m_socket.read(buffer, count, error);
}

void NetworkConnectionsManager::ManagedSocket::write(const char* buffer, int count, Error& error)
{
    m_socket.write(buffer, count, error);
}

void NetworkConnectionsManager::ManagedSocket::shutdown(Error& error)
{
    m_socket.shutdown(error);
}

void NetworkConnectionsManager::ManagedSocket::close()
{
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
    // TODO: needs to be marked non-blocking
    TCPClientSocket socket(error);
    if (error)
    {
        return;
    }

    socket.connect(address, port, error);
    if (error)
    {
        return;
    }

    m_managed_sockets.emplace_back(std::move(socket));
    m_callbacks.emplace_back(&callbacks);

    // TODO: can't assume the socket index is 0
    callbacks.onConnectionEstablished(m_managed_sockets[0]);
}

void NetworkConnectionsManager::run()
{
    // TODO: can't assume the socket index is 0
    // TODO: need to use select and iterate over all ready sockets
    ManagedSocket& socket = m_managed_sockets[0];
    ConnectionCallbacks& callbacks = *m_callbacks[0];

    // TODO: these errors need to be reported to the clients somehow
    Error todo_error;

    // TODO: buffer size and handle bigger responses
    // TODO: this only works if the server closes the connection after the response is sent. We do set the close header
    // but since we are parsing already can we double check stuff and also create APIs that support connection re-use.
    char buffer[10 * 1024];
    size_t offset = 0;
    int n = 0;
    do
    {
        n = socket.read(buffer, sizeof(buffer), todo_error);
        callbacks.onData(boost::string_view(buffer, n));
    } while ((n != 0) && !todo_error);

    // TODO: is this the correct way to shutdown here?
    // TODO: need to implement these functions in TCPClientSocket
    socket.shutdown(todo_error);
    socket.close();
}
