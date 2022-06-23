/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TESTS_IPADDRESSTESTS_HPP_
#define _ISHIKO_CPP_NETWORKING_TESTS_IPADDRESSTESTS_HPP_

#include <Ishiko/TestFramework/Core.hpp>

class IPAddressTests : public Ishiko::TestSequence
{
public:
    IPAddressTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ConstructorTest2(Ishiko::Test& test);
    static void ConstructorTest3(Ishiko::Test& test);
};

#endif
