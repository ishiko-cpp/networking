/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/TCPClientSocket.hpp"
#include "NetworkingErrorCategory.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Ishiko
{

TCPClientSocket::TCPClientSocket(Error& error) noexcept
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

TCPClientSocket::TCPClientSocket(int socket) noexcept
    : m_socket(socket)
{
}

TCPClientSocket::TCPClientSocket(TCPClientSocket&& other)
    : m_socket(other.m_socket)
{
    other.m_socket = -1;
}

TCPClientSocket::~TCPClientSocket()
{
    if (m_socket != -1)
    {
        close(m_socket);
    }
}

void TCPClientSocket::connect(IPv4Address address, Port port, Error& error)
{
    sockaddr_in linuxAddress;
    linuxAddress.sin_family = AF_INET;
    linuxAddress.sin_port = htons(port.number());
    linuxAddress.sin_addr.s_addr = htonl(address.value());

    int err = ::connect(m_socket, reinterpret_cast<sockaddr*>(&linuxAddress), sizeof(linuxAddress));
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

int TCPClientSocket::read(char* buffer, int length, Error& error)
{
    int err = recv(m_socket, buffer, length, 0);
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    return err;
}

void TCPClientSocket::write(const char* buffer, int length, Error& error)
{
    int err = send(m_socket, buffer, length, 0);
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

IPv4Address TCPClientSocket::getLocalIPAddress(Error& error) const
{
    sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    int err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    // On Linux an unconnected socket will return 0.0.0.0 while on Windows it will return an error
    if ((err == -1) || (boundAddress.sin_addr.s_addr == 0))
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    return IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
}

Port TCPClientSocket::getLocalPort(Error& error) const
{
    sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    int err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    // On Linux an unconnected socket will return 0.0.0.0 while on Windows it will return an error
    if ((err == -1) || (boundAddress.sin_addr.s_addr == 0))
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    return Port(ntohs(boundAddress.sin_port));
}

IPv4Address TCPClientSocket::getPeerIPAddress(Error& error) const
{
    sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    int err = getpeername(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    return IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
}

Port TCPClientSocket::getPeerPort(Error& error) const
{
    sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    int err = getpeername(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    return Port(ntohs(boundAddress.sin_port));
}

}
