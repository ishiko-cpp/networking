// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_HOSTNAMERESOLVERTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_HOSTNAMERESOLVERTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class HostnameResolverTests : public Ishiko::TestSequence
{
public:
    HostnameResolverTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ResolveTest1(Ishiko::Test& test);
    static void ResolveTest2(Ishiko::Test& test);
};

#endif
