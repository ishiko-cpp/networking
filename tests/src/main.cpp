/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPAddressTests.hpp"
#include "IPv4AddressTests.hpp"
#include "PortTests.hpp"
#include "TCPClientSocketTests.hpp"
#include "TCPServerSocketTests.hpp"
#include "TLSClientSocketTests.hpp"
#include "Ishiko/Networking/NetworkingLibraryInitialization.hpp"
#include "Ishiko/Networking/linkoptions.hpp"
#include <Ishiko/TestFramework/Core.hpp>

using namespace Ishiko;

int main(int argc, char* argv[])
{
    try
    {
        NetworkingLibraryInitialization networkingLibraryInitilization;

        TestHarness theTestHarness("IshikoNetworking");

        TestSequence& theTests = theTestHarness.tests();
        theTests.append<IPv4AddressTests>();
        theTests.append<IPAddressTests>();
        theTests.append<PortTests>();
        theTests.append<TCPServerSocketTests>();
        theTests.append<TCPClientSocketTests>();
        theTests.append<TLSClientSocketTests>();

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
