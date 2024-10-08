// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "NetworkConnectionsManagerTests.hpp"
#include "Ishiko/Networking/NetworkConnectionsManager.hpp"

using namespace Ishiko;

NetworkConnectionsManagerTests::NetworkConnectionsManagerTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "NetworkConnectionsManager tests", context)
{
    append<HeapAllocationErrorsTest>("Constructor test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("run test 1", RunTest1);
}

void NetworkConnectionsManagerTests::ConstructorTest1(Test& test)
{
    NetworkConnectionsManager connection_manager;

    ISHIKO_TEST_PASS();
}

void NetworkConnectionsManagerTests::RunTest1(Test& test)
{
    NetworkConnectionsManager connection_manager;

    connection_manager.run(
        [](NetworkConnectionsManager& connections_manager)
        {
            return true;
        });

    ISHIKO_TEST_PASS();
}
