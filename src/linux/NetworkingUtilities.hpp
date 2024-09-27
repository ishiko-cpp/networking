// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_LINUX_NETWORKINGUTILITIES_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_LINUX_NETWORKINGUTILITIES_HPP

#include "NativeSocketError.hpp"
#include "NetworkingErrorCategory.hpp"

namespace Ishiko
{
    class NetworkingUtilities
    {
    public:
        static NetworkingErrorCategory::Value ConvertNativeSocketError(NativeSocketError error);
    };
}

#endif
