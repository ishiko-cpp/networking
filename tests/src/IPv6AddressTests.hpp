/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TESTS_IPV6ADDRESSTESTS_HPP_
#define _ISHIKO_CPP_NETWORKING_TESTS_IPV6ADDRESSTESTS_HPP_

#include <Ishiko/TestFramework/Core.hpp>

class IPv6AddressTests : public Ishiko::TestSequence
{
public:
    IPv6AddressTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConstructorTest2(Ishiko::Test& test);
    static void ConstructorTest3(Ishiko::Test& test);
    static void LocalhostTest1(Ishiko::Test& test);
    static void ToBytesTest1(Ishiko::Test& test);
};

#endif
