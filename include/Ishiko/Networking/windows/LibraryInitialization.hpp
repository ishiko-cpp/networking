/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_WINDOWS_LIBRARYINITIALIZATION_HPP_
#define _ISHIKO_CPP_NETWORKING_WINDOWS_LIBRARYINITIALIZATION_HPP_

#include <Ishiko/Errors.hpp>

namespace Ishiko
{
namespace Networking
{

class LibraryInitialization
{
public:
    static void Startup(Error& error);
    static void Cleanup(Error& error);
};

}
}

#endif
