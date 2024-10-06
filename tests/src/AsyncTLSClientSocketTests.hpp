// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_ASYNCTLSCLIENTSOCKETTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_ASYNCTLSCLIENTSOCKETTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class AsyncTLSClientSocketTests : public Ishiko::TestSequence
{
public:
    AsyncTLSClientSocketTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConnectTest1(Ishiko::Test& test);
    static void HandshakeTest1(Ishiko::Test& test);
};

#endif
