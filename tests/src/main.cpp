// SPDX-FileCopyrightText: 2021-2024 Xavier Leclercq
// SPDX-License-Identifier: BSL-1.0

#include "AsyncTCPClientSocketTests.hpp"
#include "AsyncTLSClientSocketTests.hpp"
#include "HostnameResolverTests.hpp"
#include "IPAddressTests.hpp"
#include "IPv4AddressTests.hpp"
#include "IPv6AddressTests.hpp"
#include "NetworkConnectionsManagerTests.hpp"
#include "PortTests.hpp"
#include "TCPClientSocketTests.hpp"
#include "TCPServerSocketTests.hpp"
#include "TLSClientSocketTests.hpp"
#include "TLSServerSocketTests.hpp"
#include "Ishiko/Networking/NetworkingLibraryInitialization.hpp"
#include "Ishiko/Networking/linkoptions.hpp"
#include <Ishiko/TestFramework/Core.hpp>
#include <exception>

using namespace Ishiko;

int main(int argc, char* argv[])
{
    try
    {
        NetworkingLibraryInitialization networkingLibraryInitialization;

        TestHarness::CommandLineSpecification commandLineSpec;
        commandLineSpec.setDefaultValue("context.data", "../../data");
        commandLineSpec.setDefaultValue("context.output", "../../output");
        commandLineSpec.setDefaultValue("context.reference", "../../reference");

        Configuration configuration = commandLineSpec.createDefaultConfiguration();
        CommandLineParser::parse(commandLineSpec, argc, argv, configuration);

        TestHarness theTestHarness("Ishiko/C++ Networking Library Tests", configuration);

        TestSequence& theTests = theTestHarness.tests();
        theTests.append<IPv4AddressTests>();
        theTests.append<IPv6AddressTests>();
        theTests.append<IPAddressTests>();
        theTests.append<PortTests>();
        theTests.append<TCPServerSocketTests>();
        theTests.append<TCPClientSocketTests>();
        theTests.append<TLSServerSocketTests>();
        theTests.append<TLSClientSocketTests>();
        theTests.append<HostnameResolverTests>();
        theTests.append<NetworkConnectionsManagerTests>();
        theTests.append<AsyncTCPClientSocketTests>();
        theTests.append<AsyncTLSClientSocketTests>();

        return theTestHarness.run();
    }
    catch (const std::exception& e)
    {
        return TestApplicationReturnCode::exception;
    }
    catch (...)
    {
        return TestApplicationReturnCode::exception;
    }
}
