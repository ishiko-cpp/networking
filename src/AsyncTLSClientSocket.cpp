// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTLSClientSocket.hpp"

using namespace Ishiko;

void AsyncTLSClientSocket::Callbacks::onConnectionEstablished(NetworkConnectionsManager::ManagedTLSSocket& socket)
{
    m_socket = &socket;

    // TODO: error?
    Error error;
    onConnectionEstablished(error);
}

void AsyncTLSClientSocket::Callbacks::onHandshake()
{
}

void AsyncTLSClientSocket::Callbacks::onReadReady()
{
}

void AsyncTLSClientSocket::Callbacks::onWriteReady()
{
}

AsyncTLSClientSocket::AsyncTLSClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
    Error& error) noexcept
    : m_connections_manager{connections_manager}, m_callbacks{callbacks}
{
    // TODO: socket creation should happen here, none of that managed socket stuff
}

void AsyncTLSClientSocket::connect(IPv4Address address, Port port, const Hostname& hostname) noexcept
{
    Error error;
    m_connections_manager.connectWithTLS(address, port, hostname, m_callbacks, error);
    // TODO: even if there is no error the NetworkConnectionsManager will trigger the callback
}

void AsyncTLSClientSocket::close() noexcept
{
    m_callbacks.m_socket->close();
}
