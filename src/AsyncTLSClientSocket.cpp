// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTLSClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace Ishiko;

AsyncTLSClientSocket::AsyncTLSClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
    Error& error) noexcept
    : m_socket{SocketOption::non_blocking, error}, m_callbacks{callbacks}, m_state{State::init}
{
    if (!error)
    {
        // TODO: need to unregister somewhere
        m_registration = connections_manager.registerSocketAndCallbacks(m_socket.socket().nativeHandle(), EventHandler,
            this);
    }
}

void AsyncTLSClientSocket::connect(IPv4Address address, Port port, const Hostname& hostname) noexcept
{
    // TODO: error handling
    Error error;
    m_socket.connect(address, port, hostname.asString(), error);
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

void AsyncTLSClientSocket::handshake() noexcept
{
    // TODO: error handling
    Error error;
    m_socket.handshake(error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            // TODO: make this work with TLS
            m_state = State::waiting_for_handshake;
            m_registration.setWaitingForRead();
        }
    }
}

int AsyncTLSClientSocket::read(char* buffer, int length)
{
    // TODO: error handling
    Error error;
    int n = m_socket.read(buffer, length, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_registration.setWaitingForRead();
        }
    }
    return n;
}

void AsyncTLSClientSocket::write(const char* buffer, int length)
{
    // TODO: error handling
    Error error;
    m_socket.write(buffer, length, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_write;
            m_registration.setWaitingForWrite();
        }
    }
}

void AsyncTLSClientSocket::close() noexcept
{
    m_socket.close();
    // TODO: unregister
}

void AsyncTLSClientSocket::EventHandler(NetworkConnectionsManager::Event evt, void* data)
{
    // TODO: error handling
    Error error;

    AsyncTLSClientSocket* socket = static_cast<AsyncTLSClientSocket*>(data);
    switch (evt)
    {
    case NetworkConnectionsManager::Event::connection_ready:
        socket->m_callbacks.onConnectionEstablished(error, *socket);
        break;

    case NetworkConnectionsManager::Event::read_ready:
        if (socket->m_state == State::waiting_for_handshake)
        {
            // TODO: if the socket gets connected here it means the connect call blocked and the TLS handshaked wasn't
            // completed. Call onCallback to continue the handshake.
            socket->m_socket.onCallback();
            if (!socket->m_socket.isConnected())
            {
                // TODO
                // TLS handshake hasn't finished
                // TODO: could be waiting for a read or write, can I improve this?
                socket->m_registration.setWaitingForRead();
                // TODO: actually waiting for both definitely will cause trouble as the write will never be reset
                // TODO: I definitely need to know which to wait on
                //m_shared_state.setWaitingForWrite(this);
            }
            else
            {
                socket->m_callbacks.onHandshake(error, *socket);
            }
        }
        else
        {
            socket->m_callbacks.onReadReady(error, *socket);
        }
        break;

    case NetworkConnectionsManager::Event::write_ready:
        socket->m_callbacks.onWriteReady(error, *socket);
        break;
    }
}
