// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_LINUX_SOCKETOPTION_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_LINUX_SOCKETOPTION_HPP

#include <fcntl.h>

namespace Ishiko
{
    class SocketOption
    {
    public:
        enum Value
        {
            none = 0,
            non_blocking = O_NONBLOCK
        };
    };
}

#endif