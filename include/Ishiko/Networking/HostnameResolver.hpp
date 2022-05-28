/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_HOSTNAMERESOLVER_HPP_
#define _ISHIKO_CPP_NETWORKING_HOSTNAMERESOLVER_HPP_

#include <Ishiko/BasePlatform.h>

#if ISHIKO_OS == ISHIKO_OS_LINUX
#include "linux/HostnameResolver.hpp"
#elif ISHIKO_OS == ISHIKO_OS_WINDOWS
#include "windows/HostnameResolver.hpp"
#else
#error Unsupported or unrecognized OS
#endif

#endif
