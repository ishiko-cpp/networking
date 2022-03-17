/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_LINUX_NETWORKINGLIBRARYINITIALIZATION_HPP_
#define _ISHIKO_CPP_NETWORKING_LINUX_NETWORKINGLIBRARYINITIALIZATION_HPP_

#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class NetworkingLibraryInitialization
{
public:
    NetworkingLibraryInitialization(Error& error);
    ~NetworkingLibraryInitialization();
};

}

#endif
