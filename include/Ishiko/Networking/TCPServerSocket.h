/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TCPSERVERSOCKET_H_
#define _ISHIKO_CPP_NETWORKING_TCPSERVERSOCKET_H_

#include "IPv4Address.hpp"
#include "Port.h"
#include "TCPClientSocket.h"
#include <Ishiko/Errors.hpp>
#include <winsock2.h>

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

private:
    IPv4Address m_address;
    Port m_port;
    SOCKET m_socket;
};

}
}

#endif
