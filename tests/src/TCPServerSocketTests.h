/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_TESTS_TCPSERVERSOCKETTESTS_H_
#define _ISHIKO_NETWORKING_TESTS_TCPSERVERSOCKETTESTS_H_

#include <Ishiko/Tests/Core.hpp>

class TCPServerSocketTests : public Ishiko::Tests::TestSequence
{
public:
    TCPServerSocketTests(const Ishiko::Tests::TestNumber& number, const Ishiko::Tests::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Tests::Test& test);
};

#endif
