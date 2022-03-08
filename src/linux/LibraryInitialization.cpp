/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/LibraryInitialization.hpp"

namespace Ishiko
{
namespace Networking
{

LibraryInitialization::LibraryInitialization(Error& error)
{
    // No initialization is required on Linux
}

LibraryInitialization::~LibraryInitialization()
{
}

}
}
