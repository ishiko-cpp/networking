// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "windows/NetworkingLibraryInitialization.hpp"
#include "NetworkingErrorCategory.hpp"
#include <winsock2.h>

namespace Ishiko
{

NetworkingLibraryInitialization::NetworkingLibraryInitialization()
{
    Startup();
}

NetworkingLibraryInitialization::NetworkingLibraryInitialization(Error& error) noexcept
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

void NetworkingLibraryInitialization::Startup()
{
    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data) == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }
    else if ((LOBYTE(data.wVersion) != 2) || (HIBYTE(data.wVersion) != 2))
    {
        // TODO: more detailed error
        Throw(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
    }
}

void NetworkingLibraryInitialization::Startup(Error& error) noexcept
{
    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data) == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
    }
    else if ((LOBYTE(data.wVersion) != 2) || (HIBYTE(data.wVersion) != 2))
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
    }
}

void NetworkingLibraryInitialization::Cleanup(Error& error) noexcept
{
    if (WSACleanup() == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
    }
}

}
