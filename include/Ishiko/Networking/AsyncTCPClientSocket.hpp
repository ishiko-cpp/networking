// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
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
        class Callbacks : public NetworkConnectionsManager::ConnectionCallbacks
        {
        public:
            virtual void onConnectionEstablished(const Error& error) = 0;
            virtual void onReadReady(const Error& error) = 0;
            virtual void onWriteReady(const Error& error) = 0;

        private:
            void onConnectionEstablished(NetworkConnectionsManager::ManagedSocket& socket) override;
            void onReadReady() override;
            void onWriteReady() override;

        public: // TODO
            NetworkConnectionsManager::ManagedSocket* m_socket;
        };

        AsyncTCPClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
            Error& error) noexcept;

        void connect(IPv4Address address, Port port) noexcept;

        void close() noexcept;

    private:
        NetworkConnectionsManager& m_connections_manager;
        Callbacks& m_callbacks;
    };
}

#endif
