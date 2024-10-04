// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_TLSSERVERSOCKETTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_TLSSERVERSOCKETTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class TLSServerSocketTests : public Ishiko::TestSequence
{
public:
    TLSServerSocketTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void AcceptTest1(Ishiko::Test& test);
};

#endif
