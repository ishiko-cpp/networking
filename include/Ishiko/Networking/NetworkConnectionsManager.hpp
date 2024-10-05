// SPDX-FileCopyrightText: 2005-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP

#include "Hostname.hpp"
#include "IPv4Address.hpp"
#include "NativeSocketHandle.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include "TLSClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/Memory.hpp>
#include <boost/utility/string_view.hpp>
#include <functional>
#include <set>
#include <utility>
#include <vector>

namespace Ishiko
{
    // TODO: by design we let the clients do the reads and writes. Another possibility would have been to delegate all
    // these to the manager but that means the manager has to own a lot of buffers and also that more copying between
    // buffers may be needed.
    class NetworkConnectionsManager
    {
    public:
        enum class Event
        {
            connection_ready,
            read_ready,
            write_ready
        };

        class Registration
        {
        public:
            Registration();
            Registration(NetworkConnectionsManager* connections_manager, void* impl);

            void setWaitingForConnection();
            void setWaitingForRead();
            void setWaitingForWrite();

        private:
            NetworkConnectionsManager* m_connections_manager;
            void* m_impl;
        };

        // TODO: find a better name for this, although maybe it's OK?
        // TODO: Need to be able to close and shutdown the connection
        class ManagedTLSSocket
        {
        public:
            virtual void handshake(Error& error) = 0;

            virtual int read(char* buffer, int count, Error& error) = 0;

            virtual void write(const char* buffer, int count, Error& error) = 0;

            virtual void shutdown(Error& error) = 0;
            virtual void close() = 0;
        };

        // TODO: Can I unify this with ConnectionCallbacks or is it better like this?
        class TLSConnectionCallbacks
        {
        public:
            virtual void onConnectionEstablished(ManagedTLSSocket& socket) = 0;
            virtual void onHandshake() = 0;
            virtual void onReadReady() = 0;
            virtual void onWriteReady() = 0;
        };

        NetworkConnectionsManager();
        NetworkConnectionsManager(const NetworkConnectionsManager& other) = delete;
        NetworkConnectionsManager& operator=(const NetworkConnectionsManager& other) = delete;
        NetworkConnectionsManager(NetworkConnectionsManager&& other) = delete;
        NetworkConnectionsManager& operator=(NetworkConnectionsManager&& other) = delete;
        ~NetworkConnectionsManager() = default;

        Registration registerSocketAndCallbacks(NativeSocketHandle socket_handle,
            void (*event_handler)(Event evt, void* data), void* event_handler_data);

        void connectWithTLS(IPv4Address address, Port port, const Hostname& hostname,
            TLSConnectionCallbacks& callbacks, Error& error);

        void run(bool (*stop_function)(NetworkConnectionsManager& connections_manager));
        bool idle() const;

    private:
        class ManagedTLSSocketImpl;

        class RegistrationImpl
        {
        public:
            RegistrationImpl(NativeSocketHandle socket_handle, void (*event_handler)(Event evt, void* data),
                void* event_handler_data);

        public: // TODO
            NativeSocketHandle m_socket_handle;
            void (*m_event_handler)(Event evt, void* data);
            void* m_event_handler_data;
        };

        // TODO: the things that are shared between the manager and the sockets
        // TODO: this is an experiment: can I isolate everything that may suffer data races
        class SharedState
        {
        public:
            void setWaitingForConnection(RegistrationImpl* socket_and_callbacks);
            void setWaitingForRead(RegistrationImpl* socket_and_callbacks);
            void setWaitingForWrite(RegistrationImpl* socket_and_callbacks);

            void setWaitingForConnection(ManagedTLSSocketImpl* managed_socket);
            void setWaitingForRead(ManagedTLSSocketImpl* managed_socket);
            void setWaitingForWrite(ManagedTLSSocketImpl* managed_socket);

            std::set<RegistrationImpl*> m_new_waiting_for_connection3;
            std::set<RegistrationImpl*> m_new_waiting_for_read3;
            std::set<RegistrationImpl*> m_new_waiting_for_write3;

            std::set<ManagedTLSSocketImpl*> m_new_waiting_for_connection2;
            std::set<ManagedTLSSocketImpl*> m_new_waiting_for_read2;
            std::set<ManagedTLSSocketImpl*> m_new_waiting_for_write2;
        };

        class ManagedTLSSocketImpl : public ManagedTLSSocket
        {
        public:
            ManagedTLSSocketImpl(SharedState& shared_state, TLSClientSocket&& socket, TLSConnectionCallbacks& callbacks);

            void connect(IPv4Address address, Port port, const Hostname& hostname, Error& error);

            void handshake(Error& error) override;

            int read(char* buffer, int count, Error& error) override;

            void write(const char* buffer, int count, Error& error) override;

            void shutdown(Error& error) override;
            void close() override;

            void callback();

            TLSClientSocket& socket();

        private:
            enum class State
            {
                waiting_for_connection,
                waiting_for_handshake,
                waiting_for_read,
                waiting_for_write
            };
            SharedState& m_shared_state;
            TLSClientSocket m_socket;
            TLSConnectionCallbacks& m_callbacks;
            State m_state;
        };

        // TODO: replace this with stable collection, maybe a hive? Unless I make the clients of this class agnostic
        // of the actual memory location.
        std::vector<RegistrationImpl> m_registrations;

        // TODO: replace this with stable collection, maybe a hive? Unless I make the clients of this class agnostic
        // of the actual memory location.
        std::vector<ManagedTLSSocketImpl> m_managed_tls_sockets;
    public: // TODO: back to private
        SharedState m_shared_state;
    private:
        std::set<ManagedTLSSocketImpl*> m_waiting_for_connection2;
        std::set<ManagedTLSSocketImpl*> m_waiting_for_read2;
        std::set<ManagedTLSSocketImpl*> m_waiting_for_write2;
        std::set<RegistrationImpl*> m_waiting_for_connection3;
        std::set<RegistrationImpl*> m_waiting_for_read3;
        std::set<RegistrationImpl*> m_waiting_for_write3;
    };
}

#endif
