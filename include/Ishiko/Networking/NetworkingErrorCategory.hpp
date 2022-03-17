/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_NETWORKINGERRORCATEGORY_HPP_
#define _ISHIKO_CPP_NETWORKING_NETWORKINGERRORCATEGORY_HPP_

#include <Ishiko/Errors.hpp>

namespace Ishiko
{

class NetworkingErrorCategory : public Ishiko::ErrorCategory
{
public:
    enum class Value
    {
        generic = -1
    };

    static const NetworkingErrorCategory& Get() noexcept;

    const char* name() const noexcept override;

private:
    NetworkingErrorCategory() noexcept = default;
};

void Fail(Error& error, NetworkingErrorCategory::Value value, const std::string& message, const char* file, int line) noexcept;

}

#endif
