/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "windows/TCPServerSocket.hpp"
#include "NetworkingErrorCategory.hpp"
#include <ws2tcpip.h>

using namespace Ishiko;

const IPv4Address TCPServerSocket::AllInterfaces = IPv4Address(0);
const Port TCPServerSocket::AnyPort = Port(0);

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port)
    : m_ipAddress(address), m_port(port)
{
    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }

    SOCKADDR_IN winsockAddress;
    winsockAddress.sin_family = AF_INET;
    winsockAddress.sin_addr.s_addr = htonl(address.value());
    winsockAddress.sin_port = htons(port.number());

    int err = bind(m_socket, (sockaddr*)&winsockAddress, sizeof(winsockAddress));
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }

    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }

    m_ipAddress = IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
    m_port = Port(ntohs(boundAddress.sin_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, SOMAXCONN);
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }
}

TCPServerSocket::TCPServerSocket(IPv6Address address, Port port)
    : m_ipAddress(address), m_port(port)
{
    m_socket = WSASocket(AF_INET6, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }

    struct sockaddr_in6 winsockAddress;
    ZeroMemory(&winsockAddress, sizeof(winsockAddress));
    winsockAddress.sin6_family = AF_INET6;
    address.value().copyTo(winsockAddress.sin6_addr.u.Byte);
    winsockAddress.sin6_port = htons(port.number());

    int err = bind(m_socket, (sockaddr*)&winsockAddress, sizeof(winsockAddress));
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }

    struct sockaddr_in6 boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }

    m_ipAddress = IPv6Address(boundAddress.sin6_addr.u.Byte);
    m_port = Port(ntohs(boundAddress.sin6_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, SOMAXCONN);
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }
}

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port, Error& error) noexcept
    : m_ipAddress(address), m_port(port)
{
    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }

    SOCKADDR_IN winsockAddress;
    winsockAddress.sin_family = AF_INET;
    winsockAddress.sin_addr.s_addr = htonl(address.value());
    winsockAddress.sin_port = htons(port.number());
    
    int err = bind(m_socket, (sockaddr*)&winsockAddress, sizeof(winsockAddress));
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }

    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }

    m_ipAddress = IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
    m_port = Port(ntohs(boundAddress.sin_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, SOMAXCONN);
    if (err == SOCKET_ERROR)
    {
        close();

        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }
}

TCPServerSocket::TCPServerSocket(TCPServerSocket&& other) noexcept
    : m_ipAddress(other.m_ipAddress), m_port(other.m_port), m_socket(other.m_socket)
{
    // TODO: not sure what I should do with the IP address and port
    m_socket = INVALID_SOCKET;
}

TCPServerSocket::~TCPServerSocket()
{
    close();
}

TCPClientSocket TCPServerSocket::accept()
{
    SOCKET clientSocket = WSAAccept(m_socket, NULL, NULL, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }
    return TCPClientSocket(clientSocket);
}

TCPClientSocket TCPServerSocket::accept(Error& error) noexcept
{
    SOCKET clientSocket = WSAAccept(m_socket, NULL, NULL, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
    }
    return TCPClientSocket(clientSocket);
}

void TCPServerSocket::close()
{
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
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
