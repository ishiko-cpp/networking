// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_ASYNCTLSCLIENTSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_ASYNCTLSCLIENTSOCKET_HPP

#include "IPv4Address.hpp"
#include "NetworkConnectionsManager.hpp"
#include "Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
    class AsyncTLSClientSocket
    {
    public:
        class Callbacks : public NetworkConnectionsManager::TLSConnectionCallbacks
        {
        public:
            virtual void onConnectionEstablished(const Error& error) = 0;

        private:
            void onConnectionEstablished(NetworkConnectionsManager::ManagedTLSSocket& socket) override;
            void onHandshake() override;
            void onReadReady() override;
            void onWriteReady() override;

        public: // TODO
            NetworkConnectionsManager::ManagedTLSSocket* m_socket;
        };

        AsyncTLSClientSocket(NetworkConnectionsManager& connections_manager, Callbacks& callbacks,
            Error& error) noexcept;
        AsyncTLSClientSocket(const AsyncTLSClientSocket& other) = delete;
        AsyncTLSClientSocket& operator=(const AsyncTLSClientSocket& other) = delete;
        AsyncTLSClientSocket(AsyncTLSClientSocket&& other) = delete;
        AsyncTLSClientSocket& operator=(AsyncTLSClientSocket&& other) = delete;
        ~AsyncTLSClientSocket() = default;

        void connect(IPv4Address address, Port port, const Hostname& hostname) noexcept;

        void close() noexcept;

    private:
        NetworkConnectionsManager& m_connections_manager;
        Callbacks& m_callbacks;
    };
}

#endif
