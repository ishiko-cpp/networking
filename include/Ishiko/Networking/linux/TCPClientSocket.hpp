/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP_
#define _ISHIKO_CPP_NETWORKING_LINUX_TCPCLIENTSOCKET_HPP_

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
    ~TCPClientSocket();
};

}
}

#endif
