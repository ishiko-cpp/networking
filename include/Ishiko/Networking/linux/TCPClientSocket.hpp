/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP_

#include "../IPv4Address.hpp"
#include "../Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
namespace Networking
{

// TODO: forbid copy, allow moves
class TCPClientSocket
{
public:
    TCPClientSocket(Error& error);
    TCPClientSocket(int socket);
    ~TCPClientSocket();

    void connect(IPv4Address address, Port port, Error& error);

private:
    int m_socket;
};

}
}

#endif
