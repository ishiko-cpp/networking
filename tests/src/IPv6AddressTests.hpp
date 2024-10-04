// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_IPV6ADDRESSTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_IPV6ADDRESSTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class IPv6AddressTests : public Ishiko::TestSequence
{
public:
    IPv6AddressTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConstructorTest2(Ishiko::Test& test);
    static void ConstructorTest3(Ishiko::Test& test);
    static void LocalhostTest1(Ishiko::Test& test);
    static void ValueTest1(Ishiko::Test& test);
};

#endif
