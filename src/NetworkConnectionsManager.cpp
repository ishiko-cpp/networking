// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManager.hpp"
#include "NetworkingErrorCategory.hpp"
#include <boost/utility/string_view.hpp>
#include <chrono>
#include <thread>
#if ISHIKO_OS == ISHIKO_OS_LINUX
#include <sys/select.h>
#endif

using namespace Ishiko;

NetworkConnectionsManager::Registration::Registration()
    : m_connections_manager{nullptr}, m_impl{nullptr}
{
}

NetworkConnectionsManager::Registration::Registration(NetworkConnectionsManager* connections_manager, void* impl)
    : m_connections_manager{connections_manager}, m_impl{impl}
{
}

void NetworkConnectionsManager::Registration::setWaitingForConnection()
{
    m_connections_manager->m_shared_state.setWaitingForConnection(static_cast<RegistrationImpl*>(m_impl));
}

void NetworkConnectionsManager::Registration::setWaitingForRead()
{
    m_connections_manager->m_shared_state.setWaitingForRead(static_cast<RegistrationImpl*>(m_impl));
}

void NetworkConnectionsManager::Registration::setWaitingForWrite()
{
    m_connections_manager->m_shared_state.setWaitingForWrite(static_cast<RegistrationImpl*>(m_impl));
}

NetworkConnectionsManager::NetworkConnectionsManager()
{
    // TODO: just to avoid reallocations however even that isn't working well
    m_registrations.reserve(100);
}

NetworkConnectionsManager::Registration NetworkConnectionsManager::registerSocketAndCallbacks(
    NativeSocketHandle socket_handle, void (*event_handler)(Event evt, void* data), void* event_handler_data)
{
    // TODO: duplicate and error management
    m_registrations.emplace_back(socket_handle, event_handler, event_handler_data);
    return Registration{this, &m_registrations.back()};
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

        for (RegistrationImpl* managed_socket : m_waiting_for_read3)
        {
            FD_SET(managed_socket->m_socket_handle, &fd_read_ready);
        }
        
        for (RegistrationImpl* managed_socket : m_waiting_for_write3)
        {
            FD_SET(managed_socket->m_socket_handle, &fd_write_ready);
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
                managed_socket->m_event_handler(Event::connection_ready, managed_socket->m_event_handler_data);
                it = m_waiting_for_connection3.erase(it);
            }
            else if (FD_ISSET(managed_socket->m_socket_handle, &fd_exception))
            {
                // TODO: report error
                managed_socket->m_event_handler(Event::connection_ready, managed_socket->m_event_handler_data);
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
                managed_socket->m_event_handler(Event::read_ready, managed_socket->m_event_handler_data);
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
                managed_socket->m_event_handler(Event::write_ready, managed_socket->m_event_handler_data);
                it = m_waiting_for_write3.erase(it);
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
    return (m_waiting_for_connection3.empty() && m_shared_state.m_new_waiting_for_connection3.empty()
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

NetworkConnectionsManager::RegistrationImpl::RegistrationImpl(NativeSocketHandle socket_handle,
    void(*event_handler)(Event evt, void* data), void* event_handler_data)
    : m_socket_handle{socket_handle}, m_event_handler{event_handler}, m_event_handler_data{event_handler_data}
{
}
