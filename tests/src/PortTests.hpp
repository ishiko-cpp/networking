// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_PORTTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_PORTTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class PortTests : public Ishiko::TestSequence
{
public:
    PortTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConstructorTest2(Ishiko::Test& test);
};

#endif
