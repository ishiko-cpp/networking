/*
    Copyright (c) 2021 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "IPAddressTests.h"
#include "IPv4AddressTests.h"
#include "TCPClientSocketTests.h"
#include "TCPServerSocketTests.h"
#include "Ishiko/Networking/linkoptions.h"
#include <Ishiko/Tests/Core.h>

using namespace Ishiko::Tests;

int main(int argc, char* argv[])
{
    TestHarness theTestHarness("IshikoNetworking");

    TestSequence& theTests = theTestHarness.tests();
    theTests.append<IPv4AddressTests>();
    theTests.append<IPAddressTests>();
    theTests.append<TCPClientSocketTests>();
    theTests.append<TCPServerSocketTests>();

    return theTestHarness.run();
}
