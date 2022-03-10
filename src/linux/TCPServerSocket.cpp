/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/TCPServerSocket.hpp"
<<<<<<< HEAD
#include "ErrorCategory.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;
=======
>>>>>>> origin/develop

namespace Ishiko
{
namespace Networking
{

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port, Error& error)
<<<<<<< HEAD
    : m_address(move(address)), m_port(move(port))
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    sockaddr_in linuxAddress;
    linuxAddress.sin_family = AF_INET;
    linuxAddress.sin_port = htons(port.number());
    linuxAddress.sin_addr.s_addr = htonl(address.value());

    int err = bind(m_socket, (sockaddr*)&linuxAddress, sizeof(linuxAddress));
    if (err == -1)
    {
        close(m_socket);
        m_socket = -1;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }

    // TODO: make backlog explicit and configurable
    err = listen(m_socket, 128);
    if (err == -1)
    {
        close(m_socket);
        m_socket = -1;

        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
        return;
    }
=======
{
    // TODO
>>>>>>> origin/develop
}

TCPServerSocket::~TCPServerSocket()
{
<<<<<<< HEAD
    if (m_socket != -1)
    {
        close(m_socket);
    }
=======
>>>>>>> origin/develop
}

TCPClientSocket TCPServerSocket::accept(Error& error)
{
<<<<<<< HEAD
    int clientSocket = ::accept(m_socket, NULL, NULL);
    if (clientSocket == -1)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    return TCPClientSocket(clientSocket);
=======
    // TODO
    return TCPClientSocket(error);
>>>>>>> origin/develop
}

}
}
