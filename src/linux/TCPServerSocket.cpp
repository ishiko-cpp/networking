/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/TCPServerSocket.hpp"
#include "NetworkingErrorCategory.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace Ishiko;

const IPv4Address TCPServerSocket::AllInterfaces = IPv4Address(0);
const Port TCPServerSocket::AnyPort = Port(0);

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port)
    : m_ipAddress(std::move(address)), m_port(std::move(port))
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    sockaddr_in linuxAddress;
    linuxAddress.sin_family = AF_INET;
    linuxAddress.sin_port = htons(port.number());
    linuxAddress.sin_addr.s_addr = htonl(address.value());

    int err = bind(m_socket, (sockaddr*)&linuxAddress, sizeof(linuxAddress));
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    m_ipAddress = IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
    m_port = Port(ntohs(boundAddress.sin_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, 128);
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
}

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port, Error& error) noexcept
    : m_ipAddress(std::move(address)), m_port(std::move(port))
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
        return;
    }

    sockaddr_in linuxAddress;
    linuxAddress.sin_family = AF_INET;
    linuxAddress.sin_port = htons(port.number());
    linuxAddress.sin_addr.s_addr = htonl(address.value());

    int err = bind(m_socket, (sockaddr*)&linuxAddress, sizeof(linuxAddress));
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
        return;
    }

    sockaddr_in boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
        return;
    }

    m_ipAddress = IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
    m_port = Port(ntohs(boundAddress.sin_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, 128);
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
        return;
    }
}

TCPServerSocket::TCPServerSocket(IPv6Address address, Port port)
    : m_ipAddress(std::move(address)), m_port(std::move(port))
{
    m_socket = socket(AF_INET6, SOCK_STREAM, 0);
    if (m_socket == -1)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    sockaddr_in6 linuxAddress;
    memset(&linuxAddress, 0, sizeof(linuxAddress));
    linuxAddress.sin6_family = AF_INET6;
    address.value().copyTo(linuxAddress.sin6_addr.s6_addr);
    linuxAddress.sin6_port = htons(port.number());

    int err = bind(m_socket, (sockaddr*)&linuxAddress, sizeof(linuxAddress));
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    sockaddr_in6 boundAddress;
    socklen_t boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }

    m_ipAddress = IPv6Address(boundAddress.sin6_addr.s6_addr);
    m_port = Port(ntohs(boundAddress.sin6_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, 128);
    if (err == -1)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
}

TCPServerSocket::TCPServerSocket(TCPServerSocket&& other) noexcept
    : m_ipAddress(other.m_ipAddress), m_port(other.m_port), m_socket(other.m_socket)
{
    // TODO: not sure what I should do with the IP address and port
    m_socket = -1;
}

TCPServerSocket::~TCPServerSocket()
{
    close();
}

TCPClientSocket TCPServerSocket::accept()
{
    int clientSocket = ::accept(m_socket, NULL, NULL);
    if (clientSocket == -1)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    return TCPClientSocket(clientSocket);
}

TCPClientSocket TCPServerSocket::accept(Error& error) noexcept
{
    int clientSocket = ::accept(m_socket, NULL, NULL);
    if (clientSocket == -1)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__, error);
    }
    return TCPClientSocket(clientSocket);
}

void TCPServerSocket::close()
{
    if (m_socket != -1)
    {
        ::close(m_socket);
        m_socket = -1;
    }
}

IPAddress TCPServerSocket::ipAddress() const
{
    return m_ipAddress;
}

Port TCPServerSocket::port() const
{
    return m_port;
}
