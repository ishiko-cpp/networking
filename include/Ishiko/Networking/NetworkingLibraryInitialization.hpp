/*
    Copyright (c) 2022-2023 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NETWORKINGLIBRARYINITIALIZATION_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NETWORKINGLIBRARYINITIALIZATION_HPP

#include <Ishiko/BasePlatform.hpp>

#if ISHIKO_OS == ISHIKO_OS_LINUX
    #include "linux/NetworkingLibraryInitialization.hpp"
#elif ISHIKO_OS == ISHIKO_OS_WINDOWS
    #include "windows/NetworkingLibraryInitialization.hpp"
#else
    #error Unsupported or unrecognized OS
#endif

#endif
