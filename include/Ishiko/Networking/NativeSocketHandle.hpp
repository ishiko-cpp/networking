// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NATIVESOCKETHANDLE_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NATIVESOCKETHANDLE_HPP

#include <Ishiko/BasePlatform.hpp>

#if ISHIKO_OS == ISHIKO_OS_LINUX
#include "linux/NativeSocketHandle.hpp"
#elif ISHIKO_OS == ISHIKO_OS_WINDOWS
#include "windows/NativeSocketHandle.hpp"
#else
#error Unsupported or unrecognized OS
#endif

#endif
