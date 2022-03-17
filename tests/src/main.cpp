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
#include "Ishiko/Networking/NetworkingLibraryInitialization.hpp"
#include "Ishiko/Networking/linkoptions.hpp"
#include <Ishiko/Tests/Core.hpp>

using namespace Ishiko;
using namespace Ishiko::Tests;

int main(int argc, char* argv[])
{
    // TODO: handle the error
    Error error;
    NetworkingLibraryInitialization networkingLibraryInitilization(error);

    TestHarness theTestHarness("IshikoNetworking");

    TestSequence& theTests = theTestHarness.tests();
    theTests.append<IPv4AddressTests>();
    theTests.append<IPAddressTests>();
    theTests.append<PortTests>();
    theTests.append<TCPServerSocketTests>();
    theTests.append<TCPClientSocketTests>();

    return theTestHarness.run();
}
