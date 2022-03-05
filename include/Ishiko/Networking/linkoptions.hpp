/*
    Copyright (c) 2021-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/ishiko-cpp/networking/blob/main/LICENSE.txt
*/

#ifndef _ISHIKO_NETWORKING_LINKOPTIONS_HPP_
#define _ISHIKO_NETWORKING_LINKOPTIONS_HPP_

#ifdef _DEBUG
#pragma comment(lib, "IshikoNetworking-d.lib")
#else
#pragma comment(lib, "IshikoNetworking.lib")
#endif
#pragma comment(lib, "ws2_32.lib")

#endif
