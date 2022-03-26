/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "windows/TCPServerSocket.hpp"
#include "NetworkingErrorCategory.hpp"

using namespace std;

namespace Ishiko
{

const IPv4Address TCPServerSocket::AllInterfaces = IPv4Address(0);
const Port TCPServerSocket::AnyPort = Port(0);

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port, Error& error)
    : m_ipAddress(address), m_port(port)
{
    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_socket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    SOCKADDR_IN winsockAddress;
    winsockAddress.sin_family = AF_INET;
    winsockAddress.sin_addr.s_addr = htonl(address.value());
    winsockAddress.sin_port = htons(port.number());
    
    int err = bind(m_socket, (sockaddr*)&winsockAddress, sizeof(winsockAddress));
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    SOCKADDR_IN boundAddress;
    int boundAddressLength = sizeof(boundAddress);
    err = getsockname(m_socket, (sockaddr*)&boundAddress, &boundAddressLength);
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;\

        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    m_ipAddress = IPv4Address(ntohl(boundAddress.sin_addr.s_addr));
    m_port = Port(ntohs(boundAddress.sin_port));

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, SOMAXCONN);
    if (err == SOCKET_ERROR)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;

        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

TCPServerSocket::~TCPServerSocket()
{
    close();
}

TCPClientSocket TCPServerSocket::accept(Error& error)
{
    SOCKET clientSocket = WSAAccept(m_socket, NULL, NULL, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
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

IPv4Address TCPServerSocket::ipAddress() const
{
    return m_ipAddress;
}

Port TCPServerSocket::port() const
{
    return m_port;
}

}
