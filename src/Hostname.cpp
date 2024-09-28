// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "Hostname.hpp"

using namespace Ishiko;

Hostname::Hostname(const char* hostname)
    : m_data{hostname}
{
}

Hostname::Hostname(std::string&& hostname)
    : m_data{std::move(hostname)}
{
}

const std::string& Hostname::asString() const
{
    return m_data;
}
