/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TESTS_TCPCLIENTSOCKETTESTS_HPP_
#define _ISHIKO_CPP_NETWORKING_TESTS_TCPCLIENTSOCKETTESTS_HPP_

#include <Ishiko/Tests/Core.hpp>

class TCPClientSocketTests : public Ishiko::Tests::TestSequence
{
public:
    TCPClientSocketTests(const Ishiko::Tests::TestNumber& number, const Ishiko::Tests::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Tests::Test& test);
};

#endif
