// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_NETWORKINGERRORCATEGORY_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_NETWORKINGERRORCATEGORY_HPP

#include "NativeSocketError.hpp"
#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class NetworkingErrorCategory : public Ishiko::ErrorCategory
{
public:
    enum class Value
    {
        generic_error = -1,
        would_block = -2
    };

    static const NetworkingErrorCategory& Get() noexcept;

    const char* name() const noexcept override;
    const char* message(int ev, char* buffer, size_t len) const noexcept override;

private:
    NetworkingErrorCategory() noexcept = default;
};

void Throw(NetworkingErrorCategory::Value value, const std::string& message, const char* file, int line);
void Fail(NetworkingErrorCategory::Value value, const std::string& message, const char* file, int line,
    Error& error) noexcept;
void Fail(NativeSocketError native_error, const std::string& message, const char* file, int line,
    Error& error) noexcept;

}

#endif
