/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocketTests.hpp"
#include "Ishiko/Networking/TLSClientSocket.hpp"

using namespace Ishiko;

TLSClientSocketTests::TLSClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TLSClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void TLSClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TLSClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}
