/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_WINDOWS_TCPSERVERSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_WINDOWS_TCPSERVERSOCKET_HPP_

#include "../IPv4Address.hpp"
#include "../Port.hpp"
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

    TCPServerSocket(IPv4Address address, Port port);
    TCPServerSocket(IPv4Address address, Port port, Error& error) noexcept;
    TCPServerSocket(const TCPServerSocket& other) = delete;
    TCPServerSocket(TCPServerSocket&& other) noexcept;
    ~TCPServerSocket();

    TCPClientSocket accept();
    TCPClientSocket accept(Error& error) noexcept;
    void close();

    IPv4Address ipAddress() const;
    Port port() const;

private:
    IPv4Address m_ipAddress;
    Port m_port;
    SOCKET m_socket;
};

}

#endif
