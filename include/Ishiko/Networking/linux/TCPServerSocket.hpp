// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef _ISHIKO_CPP_NETWORKING_LINUX_TCPSERVERSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_LINUX_TCPSERVERSOCKET_HPP_

#include "../IPAddress.hpp"
#include "../IPv4Address.hpp"
#include "../IPv6Address.hpp"
#include "../SocketOption.hpp"
#include "../linux/TCPClientSocket.hpp"
#include "../Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{

// TODO: forbid copy, allow moves
class TCPServerSocket
{
public:
    static const IPv4Address AllInterfaces;
    static const Port AnyPort;

    TCPServerSocket(IPv4Address address, Port port, int socket_options = SocketOption::none);
    TCPServerSocket(IPv4Address address, Port port, int socket_options, Error& error) noexcept;
    TCPServerSocket(IPv6Address address, Port port, int socket_options = SocketOption::none);
    TCPServerSocket(const TCPServerSocket& other) = delete;
    TCPServerSocket(TCPServerSocket&& other) noexcept;
    ~TCPServerSocket();

    TCPClientSocket accept();
    TCPClientSocket accept(Error& error) noexcept;
    void close();

    IPAddress ipAddress() const;
    Port port() const;

private:
    IPAddress m_ipAddress;
    Port m_port;
    int m_socket;
};

}

#endif
