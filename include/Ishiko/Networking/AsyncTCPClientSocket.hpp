// SPDX-FileCopyrightText: 2005-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_ASYNCTCPCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_ASYNCTCPCLIENTSOCKET_HPP

#include "IPv4Address.hpp"
#include "NetworkConnectionsManager.hpp"
#include "Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
    class AsyncTCPClientSocket
    {
    public:
        class Callbacks
        {
        public:
            virtual void onConnectionEstablished(const Error& error, AsyncTCPClientSocket& socket) = 0;
            virtual void onReadReady(const Error& error, AsyncTCPClientSocket& socket) = 0;
            virtual void onWriteReady(const Error& error, AsyncTCPClientSocket& socket) = 0;
        };

        AsyncTCPClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
            Error& error) noexcept;
        AsyncTCPClientSocket(const AsyncTCPClientSocket& other) = delete;
        AsyncTCPClientSocket& operator=(const AsyncTCPClientSocket& other) = delete;
        AsyncTCPClientSocket(AsyncTCPClientSocket&& other) = delete;
        AsyncTCPClientSocket& operator=(AsyncTCPClientSocket&& other) = delete;
        ~AsyncTCPClientSocket() = default;

        void connect(IPv4Address address, Port port) noexcept;

        int read(char* buffer, int count);

        void write(const char* buffer, int count);

        void close() noexcept;

    private:
        static void EventHandler(NetworkConnectionsManager::Event evt, void* data);

        TCPClientSocket m_socket;
        NetworkConnectionsManager::Registration m_registration;
        Callbacks& m_callbacks;
    };
}

#endif
