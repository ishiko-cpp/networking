// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP

#include "Hostname.hpp"
#include "IPv4Address.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include "TLSClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/Memory.hpp>
#include <boost/utility/string_view.hpp>
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
        // TODO: find a better name for this, although maybe it's OK?
        // TODO: Need to be able to close and shutdown the connection
        class ManagedSocket
        {
        public:
            virtual int read(ByteBuffer& buffer, size_t count, Error& error) = 0;
            virtual int read(char* buffer, int count, Error& error) = 0;

            virtual void write(const char* buffer, int count, Error& error) = 0;

            virtual void shutdown(Error& error) = 0;
            virtual void close() = 0;
        };

        class ConnectionCallbacks
        {
        public:
            virtual void onConnectionEstablished(ManagedSocket& socket) = 0;
            virtual void onReadReady() = 0;
            virtual void onWriteReady() = 0;
        };

        // TODO: find a better name for this, although maybe it's OK?
        // TODO: Need to be able to close and shutdown the connection
        class ManagedTLSSocket
        {
        public:
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
            virtual void onReadReady() = 0;
            virtual void onWriteReady() = 0;
        };

        NetworkConnectionsManager();

        void connect(IPv4Address address, Port port, ConnectionCallbacks& callbacks, Error& error);
        void connectWithTLS(IPv4Address address, Port port, const Hostname& hostname,
            TLSConnectionCallbacks& callbacks, Error& error);

        void run();

    private:
        class ManagedSocketImpl;
        class ManagedTLSSocketImpl;

        // TODO: the things that are shared between the manager and the sockets
        // TODO: this is an experiment: can I isolate everything that may suffer data races
        class SharedState
        {
        public:
            void setWaitingForRead(ManagedSocketImpl* managed_socket);
            void setWaitingForWrite(ManagedSocketImpl* managed_socket);
            void setWaitingForException(ManagedSocketImpl* managed_socket);
            void setWaitingForRead(ManagedTLSSocketImpl* managed_socket);
            void setWaitingForWrite(ManagedTLSSocketImpl* managed_socket);
            void setWaitingForException(ManagedTLSSocketImpl* managed_socket);

            std::set<ManagedSocketImpl*> m_waiting_for_read;
            std::set<ManagedSocketImpl*> m_waiting_for_write;
            std::set<ManagedSocketImpl*> m_waiting_for_exception;
            std::set<ManagedTLSSocketImpl*> m_waiting_for_read2;
            std::set<ManagedTLSSocketImpl*> m_waiting_for_write2;
            std::set<ManagedTLSSocketImpl*> m_waiting_for_exception2;
        };

        class ManagedSocketImpl : public ManagedSocket
        {
        public:
            ManagedSocketImpl(SharedState& shared_state, TCPClientSocket&& socket, ConnectionCallbacks& callbacks);

            void connect(IPv4Address address, Port port);

            int read(ByteBuffer& buffer, size_t count, Error& error) override;
            int read(char* buffer, int count, Error& error) override;

            void write(const char* buffer, int count, Error& error) override;

            void shutdown(Error& error) override;
            void close() override;

            void callback();

            TCPClientSocket& socket();

        private:
            enum class State
            {
                waiting_for_connection,
                waiting_for_read,
                waiting_for_write
            };
            SharedState& m_shared_state;
            TCPClientSocket m_socket;
            ConnectionCallbacks& m_callbacks;
            State m_state;
        };

        class ManagedTLSSocketImpl : public ManagedTLSSocket
        {
        public:
            ManagedTLSSocketImpl(SharedState& shared_state, TLSClientSocket&& socket, TLSConnectionCallbacks& callbacks);

            void connect(IPv4Address address, Port port, const Hostname& hostname);

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
        std::vector<ManagedSocketImpl> m_managed_sockets;
        // TODO: replace this with stable collection, maybe a hive? Unless I make the clients of this class agnostic
        // of the actual memory location.
        std::vector<ManagedTLSSocketImpl> m_managed_tls_sockets;
        SharedState m_shared_state;
    };
}

#endif
