// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

void AsyncTCPClientSocket::Callbacks::onConnectionEstablished()
{
    // TODO: error?
    Error error;
    onConnectionEstablished(error);
}

void AsyncTCPClientSocket::Callbacks::onReadReady()
{
}

void AsyncTCPClientSocket::Callbacks::onWriteReady()
{
}

AsyncTCPClientSocket::AsyncTCPClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
    Error& error) noexcept
    : m_socket{SocketOption::non_blocking, error}, m_connections_manager{connections_manager},
    m_registration{nullptr, nullptr}, m_callbacks{callbacks}
{
    if (!error)
    {
        // TODO: need to unregister somewhere
        m_registration = m_connections_manager.registerSocketAndCallbacks(m_socket.nativeHandle(), m_callbacks);
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

void AsyncTCPClientSocket::close() noexcept
{
    m_socket.close();
    // TODO: unregister
}
