// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NETWORKCONNECTIONSMANAGER_HPP

#include "IPv4Address.hpp"
#include "Port.hpp"
#include "TCPClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/Memory.hpp>
#include <boost/utility/string_view.hpp>
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
        // TODO: needs to be able to write but what about the read? Because that can be part of events?
        // TODO: maybe it should simple read from there as well since all I'll get from select is an event saying
        // there is data to read.
        class ManagedSocket
        {
        public:
            ManagedSocket(TCPClientSocket&& socket);

            int read(ByteBuffer& buffer, size_t count, Error& error);
            int read(char* buffer, int count, Error& error);

            void write(const char* buffer, int count, Error& error);

            void shutdown(Error& error);
            void close();

        private:
            TCPClientSocket m_socket;
        };

        class ConnectionCallbacks
        {
        public:
            // TODO: passing a client socket here is wrong. I need something with a tailored interface and certainly not
            // a connect and close functions.
            // TODO: this should be useful for the client to check what it is connected to, various other parameters
            virtual void onConnectionEstablished(ManagedSocket& socket) = 0;

            virtual void onData(boost::string_view data) = 0;

            // TODO: a better name for this. A client should just try to write on the socket and if it gets an error
            // that the call would be blocking then wait for this event. Note that as usual a client must also be ready
            // for partial writes but in the case of a partial write he can then immediately try to write the rest (but
            // probably then with a higher probability of a would block error).
            virtual void onWrite() = 0;
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
        std::vector<ManagedSocket> m_managed_sockets;
        std::vector<ConnectionCallbacks*> m_callbacks;
    };
}

#endif
