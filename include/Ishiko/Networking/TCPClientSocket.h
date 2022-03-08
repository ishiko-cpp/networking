/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TCPCLIENTSOCKET_H_
#define _ISHIKO_CPP_NETWORKING_TCPCLIENTSOCKET_H_

#include "IPv4Address.hpp"
#include "Port.h"
#include <Ishiko/Errors.hpp>
#include <winsock2.h>

namespace Ishiko
{
namespace Networking
{

// TODO: forbid copy, allow moves
class TCPClientSocket
{
public:
    TCPClientSocket();
    TCPClientSocket(SOCKET socket);
    ~TCPClientSocket();

    void connect(IPv4Address address, Port port, Error& error);

private:
    SOCKET m_socket;
};

}
}

#endif
