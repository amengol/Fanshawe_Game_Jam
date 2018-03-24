#pragma once
#include <Windows.h>

struct sThreadInfo
{
    sThreadInfo() :
        handle(0), address(0)
    {
    };
    HANDLE handle;
    DWORD address;
};