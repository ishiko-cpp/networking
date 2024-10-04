// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_TLSCLIENTSOCKETTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_TLSCLIENTSOCKETTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class TLSClientSocketTests : public Ishiko::TestSequence
{
public:
    TLSClientSocketTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void WriteTest1(Ishiko::Test& test);
    static void ReadTest1(Ishiko::Test& test);
};

#endif
