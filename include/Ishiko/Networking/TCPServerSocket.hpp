/*
    Copyright (c) 2021-2023 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TCPSERVERSOCKET_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TCPSERVERSOCKET_HPP

#include <Ishiko/BasePlatform.hpp>

#if ISHIKO_OS == ISHIKO_OS_LINUX
#include "linux/TCPServerSocket.hpp"
#elif ISHIKO_OS == ISHIKO_OS_WINDOWS
#include "windows/TCPServerSocket.hpp"
#else
#error Unsupported or unrecognized OS
#endif

#endif
