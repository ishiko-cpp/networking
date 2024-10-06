// SPDX-FileCopyrightText: 2005-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_ASYNCTLSCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_ASYNCTLSCLIENTSOCKET_HPP

#include "IPv4Address.hpp"
#include "NetworkConnectionsManager.hpp"
#include "Port.hpp"
#include "TLSClientSocket.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
    class AsyncTLSClientSocket
    {
    public:
        class Callbacks
        {
        public:
            virtual void onConnectionEstablished(const Error& error, AsyncTLSClientSocket& socket) = 0;
            virtual void onHandshake(const Error& error, AsyncTLSClientSocket& socket) = 0;
            virtual void onReadReady(const Error& error, AsyncTLSClientSocket& socket) = 0;
            virtual void onWriteReady(const Error& error, AsyncTLSClientSocket& socket) = 0;
        };

        AsyncTLSClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
            Error& error) noexcept;
        AsyncTLSClientSocket(const AsyncTLSClientSocket& other) = delete;
        AsyncTLSClientSocket& operator=(const AsyncTLSClientSocket& other) = delete;
        AsyncTLSClientSocket(AsyncTLSClientSocket&& other) = delete;
        AsyncTLSClientSocket& operator=(AsyncTLSClientSocket&& other) = delete;
        ~AsyncTLSClientSocket() = default;

        void connect(IPv4Address address, Port port, const Hostname& hostname) noexcept;

        void handshake() noexcept;

        int read(char* buffer, int length);

        void write(const char* buffer, int length);

        void close() noexcept;

    private:
        static void EventHandler(NetworkConnectionsManager::Event evt, void* data);

        enum class State
        {
            init,
            waiting_for_connection,
            waiting_for_handshake,
            waiting_for_read,
            waiting_for_write
        };

        TLSClientSocket m_socket;
        NetworkConnectionsManager::Registration m_registration;
        Callbacks& m_callbacks;
        State m_state;
    };
}

#endif
