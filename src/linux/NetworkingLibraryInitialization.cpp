/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "linux/NetworkingLibraryInitialization.hpp"

namespace Ishiko
{

NetworkingLibraryInitialization::NetworkingLibraryInitialization()
{
    // No initialization is required on Linux
}

NetworkingLibraryInitialization::NetworkingLibraryInitialization(Error& error) noexcept
{
    // No initialization is required on Linux
}

NetworkingLibraryInitialization::~NetworkingLibraryInitialization()
{
}

}
