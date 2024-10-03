// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include "NetworkingErrorCategory.hpp"
#include <boost/utility/string_view.hpp>
#include <chrono>
#include <thread>
#include <iostream> // TODO: remove

using namespace Ishiko;

NetworkConnectionsManager::NetworkConnectionsManager()
{
    // TODO: just to avoid reallocations however even that isn't working well
    m_managed_sockets.reserve(100);
}

void NetworkConnectionsManager::connect(IPv4Address address, Port port, ConnectionCallbacks& callbacks, Error& error)
{
    TCPClientSocket socket(SocketOption::non_blocking, error);
    if (error)
    {
        return;
    }

    // TODO: can throw
    // TODO: not thread-safe
    m_managed_sockets.emplace_back(m_shared_state, std::move(socket), callbacks);
    m_managed_sockets.back().connect(address, port);
}

void NetworkConnectionsManager::connectWithTLS(IPv4Address address, Port port, const Hostname& hostname,
    TLSConnectionCallbacks& callbacks, Error& error)
{
    TLSClientSocket socket(SocketOption::non_blocking, error);
    if (error)
    {
        return;
    }

    // TODO: can throw
    // TODO: not thread-safe
    m_managed_tls_sockets.emplace_back(m_shared_state, std::move(socket), callbacks);
    m_managed_tls_sockets.back().connect(address, port, hostname);
}

void NetworkConnectionsManager::run()
{
    // TODO: proper way to terminate
    size_t hack = 50;
    while (--hack)
    {
        // We take a copy of the current state of the waiting lists and clear the originals. This allows the callbacks
        // to add sockets to the list without interference.
        // TODO: should I actually convert this to a vector?
        // TODO: thread safety
        std::set<ManagedSocketImpl*> waiting_for_read{m_shared_state.m_waiting_for_read};
        m_shared_state.m_waiting_for_read.clear();
        std::set<ManagedSocketImpl*> waiting_for_write{m_shared_state.m_waiting_for_write};
        m_shared_state.m_waiting_for_write.clear();
        std::set<ManagedSocketImpl*> waiting_for_exception{m_shared_state.m_waiting_for_exception};
        m_shared_state.m_waiting_for_exception.clear();
        std::set<ManagedTLSSocketImpl*> waiting_for_read2{m_shared_state.m_waiting_for_read2};
        m_shared_state.m_waiting_for_read2.clear();
        std::set<ManagedTLSSocketImpl*> waiting_for_write2{m_shared_state.m_waiting_for_write2};
        m_shared_state.m_waiting_for_write2.clear();
        std::set<ManagedTLSSocketImpl*> waiting_for_exception2{m_shared_state.m_waiting_for_exception2};
        m_shared_state.m_waiting_for_exception2.clear();

        if (waiting_for_read.empty() && waiting_for_write.empty() && waiting_for_exception.empty()
            && waiting_for_read2.empty() && waiting_for_write2.empty() && waiting_for_exception2.empty())
        {
            // TODO: for now sleep but should block or something
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        fd_set fd_read_ready;
        FD_ZERO(&fd_read_ready);
        for (ManagedSocketImpl* managed_socket : waiting_for_read)
        {
            FD_SET(managed_socket->socket().nativeHandle(), &fd_read_ready);
        }
        for (ManagedTLSSocketImpl* managed_socket : waiting_for_read2)
        {
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_read_ready);
        }

        fd_set fd_write_ready;
        FD_ZERO(&fd_write_ready);
        for (ManagedSocketImpl* managed_socket :waiting_for_write)
        {
            FD_SET(managed_socket->socket().nativeHandle(), &fd_write_ready);
        }
        for (ManagedTLSSocketImpl* managed_socket :waiting_for_write2)
        {
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_write_ready);
        }

        fd_set fd_exception;
        FD_ZERO(&fd_exception);
        for (ManagedSocketImpl* managed_socket : waiting_for_exception)
        {
            FD_SET(managed_socket->socket().nativeHandle(), &fd_exception);
        }
        for (ManagedTLSSocketImpl* managed_socket : waiting_for_exception2)
        {
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_exception);
        }

        // TODO: make this configurable?
        struct timeval stTimeOut;
        stTimeOut.tv_sec = 1;
        stTimeOut.tv_usec = 0;
        int ret = select(-1, &fd_read_ready, &fd_write_ready, &fd_exception, &stTimeOut);
        // TODO: check for ret error

        for (ManagedSocketImpl* managed_socket : waiting_for_write)
        {
            if (FD_ISSET(managed_socket->socket().nativeHandle(), &fd_write_ready))
            {
                managed_socket->callback();
            }
        }

        for (ManagedTLSSocketImpl* managed_socket : waiting_for_write2)
        {
            if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_write_ready))
            {
                managed_socket->callback();
            }
        }

        for (ManagedSocketImpl* managed_socket : waiting_for_read)
        {
            if (FD_ISSET(managed_socket->socket().nativeHandle(), &fd_read_ready))
            {
                managed_socket->callback();
            }
        }

        for (ManagedTLSSocketImpl* managed_socket : waiting_for_read2)
        {
            if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_read_ready))
            {
                managed_socket->callback();
            }
        }

        for (ManagedSocketImpl* managed_socket : waiting_for_exception)
        {
            if (FD_ISSET(managed_socket->socket().nativeHandle(), &fd_exception))
            {
                managed_socket->callback();
            }
        }

        for (ManagedTLSSocketImpl* managed_socket : waiting_for_exception2)
        {
            if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_exception))
            {
                managed_socket->callback();
            }
        }
    }
}

void NetworkConnectionsManager::SharedState::setWaitingForRead(ManagedSocketImpl* managed_socket)
{
    m_waiting_for_read.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForWrite(ManagedSocketImpl* managed_socket)
{
    m_waiting_for_write.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForException(ManagedSocketImpl* managed_socket)
{
    m_waiting_for_exception.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForRead(ManagedTLSSocketImpl* managed_socket)
{
    m_waiting_for_read2.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForWrite(ManagedTLSSocketImpl* managed_socket)
{
    m_waiting_for_write2.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForException(ManagedTLSSocketImpl* managed_socket)
{
    m_waiting_for_exception2.insert(managed_socket);
}

NetworkConnectionsManager::ManagedSocketImpl::ManagedSocketImpl(SharedState& shared_state,
    TCPClientSocket&& socket, ConnectionCallbacks& callbacks)
    : m_shared_state{shared_state}, m_socket{std::move(socket)}, m_callbacks{callbacks},
    m_state{State::waiting_for_connection}
{
}

void NetworkConnectionsManager::ManagedSocketImpl::connect(IPv4Address address, Port port)
{
    // TODO: error handling
    Error error;

    m_socket.connect(address, port, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_shared_state.setWaitingForWrite(this);
            m_shared_state.setWaitingForException(this);
        }
    }
}

int NetworkConnectionsManager::ManagedSocketImpl::read(ByteBuffer& buffer, size_t count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_shared_state.setWaitingForRead(this);
        }
    }
    return n;
}

int NetworkConnectionsManager::ManagedSocketImpl::read(char* buffer, int count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_shared_state.setWaitingForRead(this);
        }
    }
    return n;
}

void NetworkConnectionsManager::ManagedSocketImpl::write(const char* buffer, int count, Error& error)
{
    m_socket.write(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_write;
            m_shared_state.setWaitingForWrite(this);
        }
    }
}

void NetworkConnectionsManager::ManagedSocketImpl::shutdown(Error& error)
{
    m_socket.shutdown(error);
}

void NetworkConnectionsManager::ManagedSocketImpl::close()
{
    // TODO: this needs to remove the managed socket from the NetworkConnectionManager
    m_socket.close();
}

void NetworkConnectionsManager::ManagedSocketImpl::callback()
{
    // TODO: need to cacth any exceptions here
    switch (m_state)
    {
    case State::waiting_for_connection:
        m_callbacks.onConnectionEstablished(*this);
        break;

    case State::waiting_for_read:
        m_callbacks.onReadReady();
        break;

    case State::waiting_for_write:
        m_callbacks.onWriteReady();
        break;
    }
}

TCPClientSocket& NetworkConnectionsManager::ManagedSocketImpl::socket()
{
    return m_socket;
}

NetworkConnectionsManager::ManagedTLSSocketImpl::ManagedTLSSocketImpl(SharedState& shared_state,
    TLSClientSocket&& socket, TLSConnectionCallbacks& callbacks)
    : m_shared_state{shared_state}, m_socket{std::move(socket)}, m_callbacks{callbacks},
    m_state{State::waiting_for_connection}
{
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::connect(IPv4Address address, Port port, const Hostname& hostname)
{
    // TODO: error handling
    Error error;

    m_socket.connect(address, port, hostname.asString(), error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            // TODO: make this work with TLS
            // TODO: add state
            m_shared_state.setWaitingForWrite(this);
            m_shared_state.setWaitingForException(this);
        }
    }
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::handshake(Error& error)
{
    m_socket.handshake(error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            // TODO: make this work with TLS
            m_state = State::waiting_for_handshake;
            m_shared_state.setWaitingForRead(this);
        }
    }
}

int NetworkConnectionsManager::ManagedTLSSocketImpl::read(char* buffer, int count, Error& error)
{
    int n = m_socket.read(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_read;
            m_shared_state.setWaitingForRead(this);
        }
    }
    return n;
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::write(const char* buffer, int count, Error& error)
{
    m_socket.write(buffer, count, error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            m_state = State::waiting_for_write;
            m_shared_state.setWaitingForWrite(this);
        }
    }
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::shutdown(Error& error)
{
    // TODO: Do I need this on TLS sockets?
    //m_socket.shutdown(error);
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::close()
{
    // TODO: this needs to remove the managed socket from the NetworkConnectionManager
    // TODO: Do I need this on TLS sockets?
    //m_socket.close();
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::callback()
{
    // TODO: need to catch any exceptions here
    switch (m_state)
    {
    case State::waiting_for_connection:
        std::cerr << "connected" << std::endl;
        m_callbacks.onConnectionEstablished(*this);
        break;
		
    case State::waiting_for_handshake:
        // TODO: if the socket gets connected here it means the connect call blocked and the TLS handshaked wasn't
        // completed. Call onCallback to continue the handshake.
        m_socket.onCallback();
        if (!m_socket.isConnected())
        {
            // TODO
            // TLS handshake hasn't finished
            // TODO: could be waiting for a read or write, can I improve this?
            m_shared_state.setWaitingForRead(this);
            // TODO: actually waiting for both definitely will cause trouble as the write will never be reset
            // TODO: I definitely need to know which to wait on
            //m_shared_state.setWaitingForWrite(this);
        }
        else
        {
            m_callbacks.onHandshake();
        }
        break;

    case State::waiting_for_read:
        m_callbacks.onReadReady();
        break;

    case State::waiting_for_write:
        m_callbacks.onWriteReady();
        break;
    }
}

TLSClientSocket& NetworkConnectionsManager::ManagedTLSSocketImpl::socket()
{
    return m_socket;
}
