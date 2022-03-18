/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TESTS_TCPCLIENTSOCKETTESTS_HPP_
#define _ISHIKO_CPP_NETWORKING_TESTS_TCPCLIENTSOCKETTESTS_HPP_

#include <Ishiko/Tests/Core.hpp>

class TCPClientSocketTests : public Ishiko::TestSequence
{
public:
    TCPClientSocketTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void GetLocalIPAddressTest1(Ishiko::Test& test);
    static void GetLocalPortTest1(Ishiko::Test& test);
    static void GetPeerIPAddressTest1(Ishiko::Test& test);
    static void GetPeerPortTest1(Ishiko::Test& test);
    static void WriteTest1(Ishiko::Test& test);
    static void ReadTest1(Ishiko::Test& test);
};

#endif
