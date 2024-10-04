// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_TCPSERVERSOCKETTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_TCPSERVERSOCKETTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class TCPServerSocketTests : public Ishiko::TestSequence
{
public:
    TCPServerSocketTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConstructorTest2(Ishiko::Test& test);
    static void ConstructorTest3(Ishiko::Test& test);
    static void ConstructorTest4(Ishiko::Test& test);
    static void ConstructorTest5(Ishiko::Test& test);
    static void AcceptTest1(Ishiko::Test& test);
    static void AcceptTest2(Ishiko::Test& test);
    static void CloseTest1(Ishiko::Test& test);
    static void CloseTest2(Ishiko::Test& test);
};

#endif
