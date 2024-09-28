// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "linux/HostnameResolver.hpp"
#include "NetworkingErrorCategory.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>

using namespace Ishiko;

void HostnameResolver::resolve(const Hostname& name, std::vector<IPv4Address>& addresses, Error& error)
{
    return resolve(name.asString(), addresses, error);
}

void HostnameResolver::resolve(const std::string& name, std::vector<IPv4Address>& addresses, Error& error)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* result = nullptr;
    int retval = getaddrinfo(name.c_str(), "80", &hints, &result);
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
