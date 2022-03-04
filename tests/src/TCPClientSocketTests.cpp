/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPClientSocketTests.h"
#include "Ishiko/Networking/TCPClientSocket.h"

using namespace Ishiko::Networking;
using namespace Ishiko::Tests;

TCPClientSocketTests::TCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
}

void TCPClientSocketTests::ConstructorTest1(Test& test)
{
    TCPClientSocket socket;

    ISHIKO_PASS();
}
