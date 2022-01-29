/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#include "TCPClientSocketTests.h"
#include "TCPServerSocketTests.h"
#include "Ishiko/Networking/linkoptions.h"
#include <Ishiko/Tests/Core.hpp>

using namespace Ishiko::Tests;

int main(int argc, char* argv[])
{
    TestHarness theTestHarness("IshikoNetworking");

    TestSequence& theTests = theTestHarness.tests();
    theTests.append<TCPClientSocketTests>();
    theTests.append<TCPServerSocketTests>();

    return theTestHarness.run();
}
