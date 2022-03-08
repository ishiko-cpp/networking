/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "WindowsSocketLibraryInitialization.hpp"
#include "ErrorCategory.hpp"
#include <winsock2.h>

namespace Ishiko
{
namespace Networking
{

void WindowsSocketLibraryInitialization::Startup(Error& error)
{
    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data) == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
    else if ((LOBYTE(data.wVersion) != 2) || (HIBYTE(data.wVersion) != 2))
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
}

void WindowsSocketLibraryInitialization::Cleanup(Error& error)
{
    if (WSACleanup() == SOCKET_ERROR)
    {
        // TODO: more detailed error
        Fail(error, ErrorCategory::Value::generic, "", __FILE__, __LINE__);
    }
}

}
}
