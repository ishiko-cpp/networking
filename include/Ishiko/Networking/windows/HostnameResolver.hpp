/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_WINDOWS_HOSTNAMERESOLVER_HPP_
#define _ISHIKO_CPP_NETWORKING_WINDOWS_HOSTNAMERESOLVER_HPP_

#include "../IPv4Address.hpp"
#include <Ishiko/Errors.hpp>
#include <string>
#include <vector>

namespace Ishiko
{

class HostnameResolver
{
public:
    void resolve(const std::string& name, std::vector<IPv4Address>& addresses, Error& error);
};

}

#endif
