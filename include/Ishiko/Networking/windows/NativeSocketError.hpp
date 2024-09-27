// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_NATIVESOCKETERROR_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_WINDOWS_NATIVESOCKETERROR_HPP

namespace Ishiko
{
    class NativeSocketError
    {
    public:
        inline explicit NativeSocketError(int error) noexcept;

        inline operator int() const;

    private:
        int m_error;
    };
};

Ishiko::NativeSocketError::NativeSocketError(int error) noexcept
{
}

Ishiko::NativeSocketError::operator int() const
{
    return m_error;
}

#endif
