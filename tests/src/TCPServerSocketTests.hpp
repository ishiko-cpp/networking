/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TESTS_TCPSERVERSOCKETTESTS_HPP_
#define _ISHIKO_CPP_NETWORKING_TESTS_TCPSERVERSOCKETTESTS_HPP_

#include <Ishiko/Tests/Core.hpp>

class TCPServerSocketTests : public Ishiko::Tests::TestSequence
{
public:
    TCPServerSocketTests(const Ishiko::Tests::TestNumber& number, const Ishiko::Tests::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Tests::Test& test);
    static void ConstructorTest2(Ishiko::Tests::Test& test);
    static void AcceptTest1(Ishiko::Tests::Test& test);
};

#endif