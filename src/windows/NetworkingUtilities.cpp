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

    default:
        return NetworkingErrorCategory::Value::generic_error;
    }
}
