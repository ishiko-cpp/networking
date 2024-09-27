// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_SOCKETOPTION_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_SOCKETOPTION_HPP

namespace Ishiko
{
    class SocketOption
    {
    public:
        enum Value
        {
            non_blocking = 0x1
        };
    };
}

#endif
