/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TLSClientSocketTests.hpp"
#include "Ishiko/Networking/TLSClientSocket.hpp"
#include "Ishiko/Networking/TLSServerSocket.hpp"
#include <thread>

using namespace Ishiko;

TLSClientSocketTests::TLSClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "TLSClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("write test 1", WriteTest1);
}

void TLSClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    TLSClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void TLSClientSocketTests::WriteTest1(Test& test)
{
    boost::filesystem::path keyPath = test.context().getTestDataPath("test_ecdsa_key.key");
    boost::filesystem::path certificatePath = test.context().getTestDataPath("test_ecdsa_certificate.crt");

    char buffer[1];

    std::thread serverThread(
        [&keyPath, &certificatePath, &buffer]()
        {
            Error error;
            TLSServerSocket socket(IPv4Address::Localhost(), 8885, keyPath.string(), certificatePath.string(), error);
            TLSClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client writes the data
            clientSocket.read(buffer, 1, error);
        });

    // TODO: this is flaky, should be able to fix once I get async server
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Error error;
    TLSClientSocket socket(error);

    ISHIKO_TEST_FAIL_IF(error);

    socket.connect(IPv4Address::Localhost(), 8885, "example.org", error);

    ISHIKO_TEST_FAIL_IF(error);

    IPv4Address localAddress = socket.getLocalIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(localAddress, IPv4Address::Localhost());

    Port localPort = socket.getLocalPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_EQ(localPort, 0);

    IPv4Address peerAddress = socket.getPeerIPAddress(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerAddress, IPv4Address::Localhost());

    Port peerPort = socket.getPeerPort(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(peerPort, 8885);

    socket.write("a", 1, error);

    ISHIKO_TEST_FAIL_IF(error);

    serverThread.join();

    ISHIKO_TEST_FAIL_IF_NEQ(buffer[0], 'a');
    ISHIKO_TEST_PASS();
}
