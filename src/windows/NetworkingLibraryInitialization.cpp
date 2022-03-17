/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "windows/NetworkingLibraryInitialization.hpp"
#include "NetworkingErrorCategory.hpp"
#include <winsock2.h>

namespace Ishiko
{

NetworkingLibraryInitialization::NetworkingLibraryInitialization(Error& error)
{
    Startup(error);
    if (!error)
    {
        m_cleanup = true;
    }
}

NetworkingLibraryInitialization::~NetworkingLibraryInitialization()
{
    if (m_cleanup)
    {
        // In practice this shouldn't fail if WSAStartup was successful and there isn't any recovery possible anyway.
        Error ignoredError;
        Cleanup(ignoredError);
    }
}

void NetworkingLibraryInitialization::Startup(Error& error)
{
    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data) == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    else if ((LOBYTE(data.wVersion) != 2) || (HIBYTE(data.wVersion) != 2))
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
}

void NetworkingLibraryInitialization::Cleanup(Error& error)
{
    if (WSACleanup() == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(error, NetworkingErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
}

}
