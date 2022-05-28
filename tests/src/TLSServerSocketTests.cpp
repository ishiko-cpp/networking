/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSServerSocketTests.hpp"
#include "Ishiko/Networking/TLSClientSocket.hpp"
#include "Ishiko/Networking/TLSServerSocket.hpp"

using namespace Ishiko;

TLSServerSocketTests::TLSServerSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TLSServerSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("accept test 1", AcceptTest1);
}

void TLSServerSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TLSServerSocket socket(IPv4Address::Localhost(), 8785, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(socket.ipAddress(), IPv4Address::Localhost());
    ISHIKO_TEST_FAIL_IF_NEQ(socket.port(), 8785);
    ISHIKO_TEST_PASS();
}

void TLSServerSocketTests::AcceptTest1(Test& test)
{
    Error error;
    TLSServerSocket socket(IPv4Address::Localhost(), TCPServerSocket::AnyPort, error);

    Port listeningPort = socket.port();
    std::thread client(
        [listeningPort]()
        {
            Error error;
            TLSClientSocket clientSocket(error);
            clientSocket.connect(IPv4Address::Localhost(), listeningPort, "example.org", error);
        });

    TLSClientSocket clientSocket = socket.accept(error);

    ISHIKO_TEST_FAIL_IF(error);

    client.join();

    ISHIKO_TEST_PASS();
}
