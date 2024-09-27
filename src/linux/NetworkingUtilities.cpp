// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkingUtilities.hpp"

using namespace Ishiko;

NetworkingErrorCategory::Value NetworkingUtilities::ConvertNativeSocketError(NativeSocketError error)
{
    switch (error)
    {
    case EAGAIN:
        return NetworkingErrorCategory::Value::would_block;

    case EWOULDBLOCK:
        return NetworkingErrorCategory::Value::would_block;

    default:
        return NetworkingErrorCategory::Value::generic_error;
    }
}
