/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_CPP_NETWORKING_TESTS_HOSTNAMERESOLVERTESTS_HPP_
#define _ISHIKO_CPP_NETWORKING_TESTS_HOSTNAMERESOLVERTESTS_HPP_

#include <Ishiko/TestFramework/Core.hpp>

class HostnameResolverTests : public Ishiko::TestSequence
{
public:
    HostnameResolverTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ResolveTest1(Ishiko::Test& test);
    static void ResolveTest2(Ishiko::Test& test);
};

#endif
