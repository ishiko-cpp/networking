/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPClientSocketTests.hpp"
#include "Ishiko/Networking/TCPClientSocket.hpp"

using namespace Ishiko;
using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

TCPClientSocketTests::TCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void TCPClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_FAIL_IF(error);
    ISHIKO_PASS();
}
