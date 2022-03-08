/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_LINUX_TCPSERVERSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_LINUX_TCPSERVERSOCKET_HPP_

#include "../IPv4Address.hpp"
#include "../linux/TCPClientSocket.hpp"
#include "../Port.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{
namespace Networking
{

// TODO: forbid copy, allow moves
class TCPServerSocket
{
public:
    TCPServerSocket(IPv4Address address, Port port, Error& error);
    ~TCPServerSocket();

    TCPClientSocket accept(Error& error);
};

}
}

#endif
