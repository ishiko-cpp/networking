// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

void AsyncTCPClientSocket::Callbacks::onConnectionEstablished(NetworkConnectionsManager::ManagedSocket& socket)
{
    m_socket = &socket;

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
    // TODO: even if there is no error the NetworkConnectionsManager will trigger the callback
}

void AsyncTCPClientSocket::close() noexcept
{
    m_callbacks.m_socket->close();
}
