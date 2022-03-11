/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/TCPClientSocket.hpp"
#include "ErrorCategory.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Ishiko
{
namespace Networking
{

TCPClientSocket::TCPClientSocket(Error& error)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

TCPClientSocket::TCPClientSocket(int socket)
    : m_socket(socket)
{
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
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

int TCPClientSocket::read(char* buffer, int length, Error& error)
{
    int err = recv(m_socket, buffer, length, 0);
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    return err;
}

void TCPClientSocket::write(const char* buffer, int length, Error& error)
{
    int err = send(m_socket, buffer, length, 0);
    if (err == -1)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
}

}
}