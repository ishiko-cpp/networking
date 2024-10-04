// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#ifndef GUARD_ISHIKO_CPP_NETWORKING_TESTS_TCPCLIENTSOCKETTESTS_HPP
#define GUARD_ISHIKO_CPP_NETWORKING_TESTS_TCPCLIENTSOCKETTESTS_HPP

#include <Ishiko/TestFramework/Core.hpp>

class TCPClientSocketTests : public Ishiko::TestSequence
{
public:
    TCPClientSocketTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConstructorTest2(Ishiko::Test& test);
    static void GetLocalIPAddressTest1(Ishiko::Test& test);
    static void GetLocalPortTest1(Ishiko::Test& test);
    static void GetPeerIPAddressTest1(Ishiko::Test& test);
    static void GetPeerPortTest1(Ishiko::Test& test);
    static void WriteTest1(Ishiko::Test& test);
    static void ReadTest1(Ishiko::Test& test);
    static void CloseTest1(Ishiko::Test& test);
    static void ShutdownTest1(Ishiko::Test& test);
};

#endif
