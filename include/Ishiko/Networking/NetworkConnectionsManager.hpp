// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP

#include "IPv4Address.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#include <boost/utility/string_view.hpp>
#include <utility>
#include <vector>

namespace Ishiko
{
    class NetworkConnectionsManager
    {
    public:
        class ConnectionCallbacks
        {
        public:
            // TODO: passing a client socket here is wrong. I need something with a tailored interface and certainly not
            // a connect and close functions.
            // TODO: this should be useful for the client to check what it is connected to, various other parameters
            virtual void onConnectionEstablished(TCPClientSocket& socket) = 0;

            virtual void onData(boost::string_view data) = 0;
        };

        NetworkConnectionsManager();

        void connect(IPv4Address address, Port port, ConnectionCallbacks& callbacks, Error& error);

        void run();

    private:
        // TODO: remove
        Error m_temp_hack_todo;
        // TODO: replace this with stable collection, maybe a colony? Unless I make the clients of this class agnostic
        // of the actual memory location which is probably what I need to do as I don't really want to give them access
        // to the sockets but I more narrow interface.
        TCPClientSocket m_client_sockets;
        std::vector<ConnectionCallbacks*> m_callbacks;
    };
}

#endif
