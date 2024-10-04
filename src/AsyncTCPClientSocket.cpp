// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

void AsyncTCPClientSocket::Callbacks::onConnectionEstablished(NetworkConnectionsManager::ManagedSocket& socket)
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
    : m_connections_manager{connections_manager}, m_callbacks{callbacks}
{
}

void AsyncTCPClientSocket::connect(IPv4Address address, Port port) noexcept
{
    Error error;
    m_connections_manager.connect(address, port, m_callbacks, error);
    if (!error || (error.code() != NetworkingErrorCategory::Value::would_block))
    {
        // There was no error meaning the connect succeeded immediately or there was an error different than
        // 'would_block'. In either case we can call the callback immediately.
        m_callbacks.onConnectionEstablished(error);
    }
}
