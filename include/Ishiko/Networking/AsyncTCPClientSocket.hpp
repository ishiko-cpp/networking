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
        class Callbacks : public NetworkConnectionsManager::ConnectionCallbacks2
        {
        public:
            virtual void onConnectionEstablished(const Error& error) = 0;
            virtual void onReadReady(const Error& error) = 0;
            virtual void onWriteReady(const Error& error) = 0;

        private:
            void onConnectionEstablished() override;
            void onReadReady() override;
            void onWriteReady() override;
        };

        AsyncTCPClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
            Error& error) noexcept;
        AsyncTCPClientSocket(const AsyncTCPClientSocket& other) = delete;
        AsyncTCPClientSocket& operator=(const AsyncTCPClientSocket& other) = delete;
        AsyncTCPClientSocket(AsyncTCPClientSocket&& other) = delete;
        AsyncTCPClientSocket& operator=(AsyncTCPClientSocket&& other) = delete;
        ~AsyncTCPClientSocket() = default;

        void connect(IPv4Address address, Port port) noexcept;

        void close() noexcept;

    private:
        TCPClientSocket m_socket;
        // TODO: can I reduce the number of members here since a lot will be contained in the registration object
        NetworkConnectionsManager& m_connections_manager;
        NetworkConnectionsManager::Registration m_registration;
        Callbacks& m_callbacks;
    };
}

#endif
