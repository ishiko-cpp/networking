// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_HOSTNAME_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_HOSTNAME_HPP

#include <string>

namespace Ishiko
{
    class Hostname
    {
    public:
        explicit Hostname(const char* hostname);
        explicit Hostname(std::string&& hostname);

        const std::string& asString() const;

    private:
        std::string m_data;
    };
}

#endif
