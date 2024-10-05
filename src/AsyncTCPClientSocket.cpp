// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

AsyncTCPClientSocket::AsyncTCPClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
    Error& error) noexcept
    : m_socket{SocketOption::non_blocking, error}, m_callbacks{callbacks}
{
    if (!error)
    {
        // TODO: need to unregister somewhere
        m_registration = connections_manager.registerSocketAndCallbacks(m_socket.nativeHandle(), EventHandler, this);
    }
}

void AsyncTCPClientSocket::connect(IPv4Address address, Port port) noexcept
{
    // TODO: error handling
    Error error;
    m_socket.connect(address, port, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: thread safety
            m_registration.setWaitingForConnection();
        }
    }
    else
    {
        // TODO: we need to get the callback executed from the NetworkConnectionsManager::run function
    }
}

int AsyncTCPClientSocket::read(char* buffer, int count)
{
    // TODO: error handling
    Error error;
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: thread safety
            m_registration.setWaitingForRead();
        }
    }
    return n;
}

void AsyncTCPClientSocket::write(const char* buffer, int count)
{
    // TODO: error handling
    Error error;
    m_socket.write(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: thread safety
            m_registration.setWaitingForWrite();
        }
    }
}

void AsyncTCPClientSocket::close() noexcept
{
    m_socket.close();
    // TODO: unregister
}

void AsyncTCPClientSocket::EventHandler(NetworkConnectionsManager::Event evt, void* data)
{
    // TODO: error handling
    Error error;

    AsyncTCPClientSocket* socket = static_cast<AsyncTCPClientSocket*>(data);
    switch (evt)
    {
    case NetworkConnectionsManager::Event::connection_ready:
        socket->m_callbacks.onConnectionEstablished(error, *socket);
        break;

    case NetworkConnectionsManager::Event::read_ready:
        socket->m_callbacks.onReadReady(error, *socket);
        break;

    case NetworkConnectionsManager::Event::write_ready:
        socket->m_callbacks.onWriteReady(error, *socket);
        break;
    }
}
