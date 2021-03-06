/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_WINDOWS_NETWORKINGLIBRARYINITIALIZATION_HPP_
#define _ISHIKO_CPP_NETWORKING_WINDOWS_NETWORKINGLIBRARYINITIALIZATION_HPP_

#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class NetworkingLibraryInitialization
{
public:
    NetworkingLibraryInitialization();
    NetworkingLibraryInitialization(Error& error) noexcept;
    ~NetworkingLibraryInitialization();

    static void Startup();
    static void Startup(Error& error) noexcept;
    static void Cleanup(Error& error) noexcept;

private:
    bool m_cleanup;
};

}

#endif
