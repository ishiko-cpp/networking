// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

void AsyncTCPClientSocket::Callbacks::onConnectionEstablished(void* callback_data)
{
    // TODO: error?
    Error error;
    onConnectionEstablished(error, *static_cast<AsyncTCPClientSocket*>(callback_data));
}

void AsyncTCPClientSocket::Callbacks::onReadReady(void* callback_data)
{
    // TODO: error?
    Error error;
    onReadReady(error, *static_cast<AsyncTCPClientSocket*>(callback_data));
}

void AsyncTCPClientSocket::Callbacks::onWriteReady(void* callback_data)
{
    // TODO: error?
    Error error;
    onWriteReady(error, *static_cast<AsyncTCPClientSocket*>(callback_data));
}

AsyncTCPClientSocket::AsyncTCPClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
    Error& error) noexcept
    : m_socket{SocketOption::non_blocking, error}, m_connections_manager{connections_manager},
    m_registration{nullptr, nullptr}, m_callbacks{callbacks}
{
    if (!error)
    {
        // TODO: need to unregister somewhere
        m_registration = m_connections_manager.registerSocketAndCallbacks(m_socket.nativeHandle(), m_callbacks, this);
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
