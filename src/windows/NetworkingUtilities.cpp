// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkingUtilities.hpp"
#define NOMINMAX // TODO: what rubbish, how do I put this ugly windows logic somewhere central
#include <winsock2.h>

using namespace Ishiko;

NetworkingErrorCategory::Value NetworkingUtilities::ConvertNativeSocketError(NativeSocketError error)
{
    switch (error)
    {
    case WSAEWOULDBLOCK:
        return NetworkingErrorCategory::Value::would_block;

    case WSANOTINITIALISED:
        return NetworkingErrorCategory::Value::networking_library_not_initialized;

    default:
        return NetworkingErrorCategory::Value::generic_error;
    }
}
