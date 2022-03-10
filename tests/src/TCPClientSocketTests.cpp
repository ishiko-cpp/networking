/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPClientSocketTests.hpp"
#include "Ishiko/Networking/TCPClientSocket.hpp"
#include "Ishiko/Networking/TCPServerSocket.hpp"
#include <thread>

using namespace Ishiko;
using namespace Ishiko::Networking;
using namespace Ishiko::Tests;
using namespace std;

TCPClientSocketTests::TCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("write test 1", WriteTest1);
}

void TCPClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TCPClientSocket socket(error);

    ISHIKO_FAIL_IF(error);
    ISHIKO_PASS();
}

void TCPClientSocketTests::WriteTest1(Test& test)
{
    char buffer[1];
    
    thread serverThread(
        [&buffer]()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 8585, error);
            TCPClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client writes the data
            clientSocket.read(buffer, 1, error);
        }
    );

    Error error;
    TCPClientSocket socket(error);

    ISHIKO_FAIL_IF(error);

    socket.connect(IPv4Address::Localhost(), 8585, error);
    
    ISHIKO_FAIL_IF(error);

    socket.write("a", 1, error);

    ISHIKO_FAIL_IF(error);

    serverThread.join();

    ISHIKO_FAIL_IF_NEQ(buffer[0], 'a');
    ISHIKO_PASS();
}
