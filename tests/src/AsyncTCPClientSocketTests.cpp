// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocketTests.hpp"
#include "Ishiko/Networking/AsyncTCPClientSocket.hpp"
#include "Ishiko/Networking/NetworkConnectionsManager.hpp"
#include "Ishiko/Networking/TCPServerSocket.hpp"
#include <thread>

using namespace Ishiko;

namespace
{
    class TestCallbacks : public AsyncTCPClientSocket::Callbacks
    {
    public:
        TestCallbacks();

        void onConnectionEstablished(const Error& error) override;
        void onReadReady(const Error& error) override;
        void onWriteReady(const Error& error) override;

        bool connectionEstablishedCalled() const;
        ErrorCode connectionEstablishedCalledError() const;

    private:
        bool m_connection_established_called;
        ErrorCode m_connection_established_called_error;
    };

    TestCallbacks::TestCallbacks()
        : m_connection_established_called{false}
    {
    }

    void TestCallbacks::onConnectionEstablished(const Error& error)
    {
        m_connection_established_called = true;
        m_connection_established_called_error = error.code();
    }

    void TestCallbacks::onReadReady(const Error& error)
    {
    }

    void TestCallbacks::onWriteReady(const Error& error)
    {
    }

    bool TestCallbacks::connectionEstablishedCalled() const
    {
        return m_connection_established_called;
    }

    ErrorCode TestCallbacks::connectionEstablishedCalledError() const
    {
        return m_connection_established_called_error;
    }
}

AsyncTCPClientSocketTests::AsyncTCPClientSocketTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "AsyncTCPClientSocket tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("connect test 1", ConnectTest1);
}

void AsyncTCPClientSocketTests::ConstructorTest1(Test& test)
{
    Error error;
    NetworkConnectionsManager connection_manager;
    TestCallbacks callbacks;
    AsyncTCPClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_PASS();
}

void AsyncTCPClientSocketTests::ConnectTest1(Test& test)
{
    std::thread serverThread(
        []()
        {
            Error error;
            TCPServerSocket socket(IPv4Address::Localhost(), 9685, SocketOption::none, error);
            TCPClientSocket clientSocket = socket.accept(error);

            // This will also make sure the server stays alive until the client closes the connection
            //char buffer[1];
            //clientSocket.read(buffer, 1, error);
            // TODO
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    );

    Error error;
    NetworkConnectionsManager connection_manager;
    TestCallbacks callbacks;
    AsyncTCPClientSocket socket{connection_manager, callbacks, error};

    ISHIKO_TEST_ABORT_IF(error);

    // TODO: assign unique port number
    socket.connect(IPv4Address::Localhost(), 9685);

    connection_manager.run(
        [](NetworkConnectionsManager& connections_manager)
        {
            return connections_manager.idle();
        });

    //socket.close();

    serverThread.join();

    ISHIKO_TEST_FAIL_IF_NOT(callbacks.connectionEstablishedCalled());
    ISHIKO_TEST_FAIL_IF(callbacks.connectionEstablishedCalledError());
    ISHIKO_TEST_PASS();
}
