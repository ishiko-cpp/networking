// SPDX-FileCopyrightText: 2005-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_TCPSERVERSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_TCPSERVERSOCKET_HPP

#include "../IPAddress.hpp"
#include "../IPv4Address.hpp"
#include "../Port.hpp"
#include "../SocketOption.hpp"
#include "../windows/TCPClientSocket.hpp"
#include <Ishiko/Errors.hpp>
#define NOMINMAX // TODO: what rubbish, how do I put this ugly windows logic somewhere central
#include <winsock2.h>

namespace Ishiko
{
    class TCPServerSocket
    {
    public:
        static const IPv4Address AllInterfaces;
        static const Port AnyPort;

        TCPServerSocket(IPv4Address address, Port port, int socket_options = SocketOption::none);
        TCPServerSocket(IPv4Address address, Port port, int socket_options, Error& error) noexcept;
        TCPServerSocket(IPv6Address address, Port port, int socket_options = SocketOption::none);
        TCPServerSocket(const TCPServerSocket& other) = delete;
        TCPServerSocket& operator=(const TCPServerSocket& other) = delete;
        TCPServerSocket(TCPServerSocket&& other) noexcept;
        TCPServerSocket& operator=(TCPServerSocket&& other) = delete;
        ~TCPServerSocket();

        TCPClientSocket accept();
        TCPClientSocket accept(Error& error) noexcept;
        void close();

        IPAddress ipAddress() const;
        Port port() const;

    private:
        IPAddress m_ipAddress;
        Port m_port;
        SOCKET m_socket;
    };
}

#endif
