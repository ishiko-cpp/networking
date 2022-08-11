/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "windows/HostnameResolver.hpp"
#include "NetworkingErrorCategory.hpp"
#include <ws2tcpip.h>

using namespace Ishiko;

void HostnameResolver::resolve(const std::string& name, std::vector<IPv4Address>& addresses, Error& error)
{
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* result = nullptr;
    DWORD retval = getaddrinfo(name.c_str(), "80", &hints, &result);
    if (retval)
    {
        // TODO: handle error
        Fail(NetworkingErrorCategory::Value::generic_error, "", __FILE__, __LINE__, error);
        return;
    }

    struct addrinfo* ptr;
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        switch (ptr->ai_family)
        {
        case AF_INET:
            {
                struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
                addresses.emplace_back(ntohl(sockaddr_ipv4->sin_addr.s_addr));
            }
            break;

        default:
            break;
        }
    }
    
    freeaddrinfo(result);
}
