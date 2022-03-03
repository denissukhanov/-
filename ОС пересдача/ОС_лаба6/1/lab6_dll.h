#pragma once
#ifdef l6_EXPORTS
#define l6 __declspec(dllexport)
#else
#define l6 __declspec(dllimport)
#endif

#include <iostream>
#include <Windows.h>

using namespace std;

HANDLE DefaultMutex;
extern "C++" l6 DWORD WINAPI first(LPVOID);
extern "C++" l6 DWORD WINAPI second(LPVOID);