/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSServerSocket.hpp"

using namespace Ishiko;

TLSServerSocket::TLSServerSocket(IPv4Address address, Port port, Error& error) noexcept
    : m_socket(address, port, error)
{
}

TLSClientSocket TLSServerSocket::accept(Error& error) noexcept
{
    TCPClientSocket clientSocket = m_socket.accept(error);
    // TODO: handle error

    // TODO: establish TLS connection

    return TLSClientSocket(std::move(clientSocket));
}

IPv4Address TLSServerSocket::ipAddress() const
{
    return m_socket.ipAddress();
}

Port TLSServerSocket::port() const
{
    return m_socket.port();
}
