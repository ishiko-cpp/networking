/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/TCPServerSocket.hpp"

namespace Ishiko
{
namespace Networking
{

TCPServerSocket::TCPServerSocket(IPv4Address address, Port port, Error& error)
{
    // TODO
}

TCPServerSocket::~TCPServerSocket()
{
}

TCPClientSocket TCPServerSocket::accept(Error& error)
{
    // TODO
    return TCPClientSocket(error);
}

}
}
