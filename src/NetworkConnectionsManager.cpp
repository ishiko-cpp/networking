// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include "NetworkingErrorCategory.hpp"
#include <boost/utility/string_view.hpp>
#include <chrono>
#include <thread>

using namespace Ishiko;

NetworkConnectionsManager::Registration::Registration()
    : m_connections_manager{nullptr}, m_impl{nullptr}
{
}

NetworkConnectionsManager::Registration::Registration(NetworkConnectionsManager* connections_manager,
    RegistrationImpl* socket_and_callbacks)
    : m_connections_manager{connections_manager}, m_impl{socket_and_callbacks}
{
}

void NetworkConnectionsManager::Registration::setWaitingForConnection()
{
    m_connections_manager->setWaitingForConnection(m_impl);
}

void NetworkConnectionsManager::Registration::setWaitingForRead()
{
    m_connections_manager->setWaitingForRead(m_impl);
}

void NetworkConnectionsManager::Registration::setWaitingForWrite()
{
    m_connections_manager->setWaitingForWrite(m_impl);
}

NetworkConnectionsManager::NetworkConnectionsManager()
{
    // TODO: just to avoid reallocations however even that isn't working well
    m_registrations.reserve(100);
}

NetworkConnectionsManager::Registration NetworkConnectionsManager::registerSocketAndCallbacks(
    NativeSocketHandle socket_handle, ConnectionCallbacks& callbacks, void* callback_data)
{
    // TODO: duplicate and error management
    m_registrations.emplace_back(socket_handle, &callbacks, callback_data);
    return Registration{this, &m_registrations.back()};
}

void NetworkConnectionsManager::setWaitingForConnection(RegistrationImpl* registration)
{
    m_shared_state.setWaitingForConnection(registration);
}

void NetworkConnectionsManager::setWaitingForRead(RegistrationImpl* registration)
{
    m_shared_state.setWaitingForRead(registration);
}

void NetworkConnectionsManager::setWaitingForWrite(RegistrationImpl* registration)
{
    m_shared_state.setWaitingForWrite(registration);
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
    m_managed_tls_sockets.back().connect(address, port, hostname, error);
}

void NetworkConnectionsManager::run(bool (*stop_function)(NetworkConnectionsManager& connections_manager))
{
    while (!stop_function(*this))
    {
        if (idle())
        {
            // TODO: for now sleep but should block or something
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // Merge the new sockets
        // TODO: thread safety
        m_waiting_for_connection3.insert(m_shared_state.m_new_waiting_for_connection3.begin(),
            m_shared_state.m_new_waiting_for_connection3.end());
        m_shared_state.m_new_waiting_for_connection3.clear();
        m_waiting_for_read3.insert(m_shared_state.m_new_waiting_for_read3.begin(),
            m_shared_state.m_new_waiting_for_read3.end());
        m_shared_state.m_new_waiting_for_read3.clear();
        m_waiting_for_write3.insert(m_shared_state.m_new_waiting_for_write3.begin(),
            m_shared_state.m_new_waiting_for_write3.end());
        m_shared_state.m_new_waiting_for_write3.clear();

        m_waiting_for_connection2.insert(m_shared_state.m_new_waiting_for_connection2.begin(),
            m_shared_state.m_new_waiting_for_connection2.end());
        m_shared_state.m_new_waiting_for_connection2.clear();
        m_waiting_for_read2.insert(m_shared_state.m_new_waiting_for_read2.begin(),
            m_shared_state.m_new_waiting_for_read2.end());
        m_shared_state.m_new_waiting_for_read2.clear();
        m_waiting_for_write2.insert(m_shared_state.m_new_waiting_for_write2.begin(),
            m_shared_state.m_new_waiting_for_write2.end());
        m_shared_state.m_new_waiting_for_write2.clear();

        fd_set fd_read_ready;
        FD_ZERO(&fd_read_ready);
        fd_set fd_write_ready;
        FD_ZERO(&fd_write_ready);
        fd_set fd_exception;
        FD_ZERO(&fd_exception);

        for (RegistrationImpl* managed_socket : m_waiting_for_connection3)
        {
            FD_SET(managed_socket->m_socket_handle, &fd_write_ready);
            FD_SET(managed_socket->m_socket_handle, &fd_exception);
        }
        for (ManagedTLSSocketImpl* managed_socket : m_waiting_for_connection2)
        {
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_write_ready);
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_exception);
        }

        for (RegistrationImpl* managed_socket : m_waiting_for_read3)
        {
            FD_SET(managed_socket->m_socket_handle, &fd_read_ready);
        }
        for (ManagedTLSSocketImpl* managed_socket : m_waiting_for_read2)
        {
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_read_ready);
        }
        
        for (RegistrationImpl* managed_socket : m_waiting_for_write3)
        {
            FD_SET(managed_socket->m_socket_handle, &fd_write_ready);
        }
        for (ManagedTLSSocketImpl* managed_socket : m_waiting_for_write2)
        {
            FD_SET(managed_socket->socket().socket().nativeHandle(), &fd_write_ready);
        }      

        // TODO: make this configurable?
        struct timeval stTimeOut;
        stTimeOut.tv_sec = 1;
        stTimeOut.tv_usec = 0;
        int ret = select(-1, &fd_read_ready, &fd_write_ready, &fd_exception, &stTimeOut);
        // TODO: check for ret error
        // TODO: if it is 0 then timeout occurred

        for (std::set<RegistrationImpl*>::iterator it = m_waiting_for_connection3.begin(); it != m_waiting_for_connection3.end();)
        {
            RegistrationImpl* managed_socket = *it;
            if (FD_ISSET(managed_socket->m_socket_handle, &fd_write_ready))
            {
                managed_socket->m_callbacks->onConnectionEstablished(managed_socket->m_callback_data);
                it = m_waiting_for_connection3.erase(it);
            }
            else if (FD_ISSET(managed_socket->m_socket_handle, &fd_exception))
            {
                // TODO: report error
                managed_socket->m_callbacks->onConnectionEstablished(managed_socket->m_callback_data);
                it = m_waiting_for_connection3.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<RegistrationImpl*>::iterator it = m_waiting_for_read3.begin(); it != m_waiting_for_read3.end();)
        {
            RegistrationImpl* managed_socket = *it;
            if (FD_ISSET(managed_socket->m_socket_handle, &fd_read_ready))
            {
                managed_socket->m_callbacks->onReadReady(managed_socket->m_callback_data);
                it = m_waiting_for_read3.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<RegistrationImpl*>::iterator it = m_waiting_for_write3.begin(); it != m_waiting_for_write3.end();)
        {
            RegistrationImpl* managed_socket = *it;
            if (FD_ISSET(managed_socket->m_socket_handle, &fd_write_ready))
            {
                managed_socket->m_callbacks->onWriteReady(managed_socket->m_callback_data);
                it = m_waiting_for_write3.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<ManagedTLSSocketImpl*>::iterator it = m_waiting_for_connection2.begin(); it != m_waiting_for_connection2.end();)
        {
            ManagedTLSSocketImpl* managed_socket = *it;
            if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_write_ready))
            {
                managed_socket->callback();
                it = m_waiting_for_connection2.erase(it);
            }
            else if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_exception))
            {
                managed_socket->callback();
                it = m_waiting_for_connection2.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<ManagedTLSSocketImpl*>::iterator it = m_waiting_for_read2.begin(); it != m_waiting_for_read2.end();)
        {
            ManagedTLSSocketImpl* managed_socket = *it;
            if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_read_ready))
            {
                managed_socket->callback();
                it = m_waiting_for_read2.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (std::set<ManagedTLSSocketImpl*>::iterator it = m_waiting_for_write2.begin(); it != m_waiting_for_write2.end();)
        {
            ManagedTLSSocketImpl* managed_socket = *it;
            if (FD_ISSET(managed_socket->socket().socket().nativeHandle(), &fd_write_ready))
            {
                managed_socket->callback();
                it = m_waiting_for_write2.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

bool NetworkConnectionsManager::idle() const
{
    return (m_shared_state.m_new_waiting_for_read2.empty() && m_shared_state.m_new_waiting_for_write2.empty()
        && m_shared_state.m_new_waiting_for_connection2.empty() && m_waiting_for_connection2.empty()
        && m_waiting_for_read2.empty() && m_waiting_for_write2.empty()
        && m_waiting_for_connection3.empty() && m_shared_state.m_new_waiting_for_connection3.empty()
        && m_waiting_for_read3.empty() && m_shared_state.m_new_waiting_for_read3.empty()
        && m_waiting_for_write3.empty() && m_shared_state.m_new_waiting_for_write3.empty());
}

void NetworkConnectionsManager::SharedState::setWaitingForConnection(RegistrationImpl* socket_and_callbacks)
{
    m_new_waiting_for_connection3.insert(socket_and_callbacks);
}

void NetworkConnectionsManager::SharedState::setWaitingForRead(RegistrationImpl* socket_and_callbacks)
{
    m_new_waiting_for_read3.insert(socket_and_callbacks);
}

void NetworkConnectionsManager::SharedState::setWaitingForWrite(RegistrationImpl* socket_and_callbacks)
{
    m_new_waiting_for_write3.insert(socket_and_callbacks);
}

void NetworkConnectionsManager::SharedState::setWaitingForConnection(ManagedTLSSocketImpl* managed_socket)
{
    m_new_waiting_for_connection2.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForRead(ManagedTLSSocketImpl* managed_socket)
{
    m_new_waiting_for_read2.insert(managed_socket);
}

void NetworkConnectionsManager::SharedState::setWaitingForWrite(ManagedTLSSocketImpl* managed_socket)
{
    m_new_waiting_for_write2.insert(managed_socket);
}

NetworkConnectionsManager::ManagedTLSSocketImpl::ManagedTLSSocketImpl(SharedState& shared_state,
    TLSClientSocket&& socket, TLSConnectionCallbacks& callbacks)
    : m_shared_state{shared_state}, m_socket{std::move(socket)}, m_callbacks{callbacks},
    m_state{State::waiting_for_connection}
{
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::connect(IPv4Address address, Port port, const Hostname& hostname,
    Error& error)
{
    m_socket.connect(address, port, hostname.asString(), error);
    if (error)
    {
        if (error.code() == NetworkingErrorCategory::Value::would_block)
        {
            // TODO: not sure but this may cause a race condition with run() since state and the call are separate steps
            // TODO: make this work with TLS
            m_shared_state.setWaitingForConnection(this);
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
    m_socket.close();
}

void NetworkConnectionsManager::ManagedTLSSocketImpl::callback()
{
    // TODO: need to catch any exceptions here
    switch (m_state)
    {
    case State::waiting_for_connection:
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

NetworkConnectionsManager::RegistrationImpl::RegistrationImpl(NativeSocketHandle socket_handle,
    ConnectionCallbacks* callbacks, void* callback_data)
    : m_socket_handle{socket_handle}, m_callbacks{callbacks}, m_callback_data{callback_data}
{
}
