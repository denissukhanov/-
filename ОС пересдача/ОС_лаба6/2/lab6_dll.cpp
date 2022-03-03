// #include "pch.h"
#include "lab6_dll.h"

DWORD WINAPI first(LPVOID a)
{
	for (int i = 0; i < 10; i++)
	{

		WaitForSingleObject(DefaultMutex, INFINITE);

		cout << "E\n";
		Sleep(100);
		cout << "V\n";
		Sleep(100);
		cout << "A\n";
		Sleep(100);
		ReleaseMutex(DefaultMutex);
	}
	return 0;
}

DWORD WINAPI second(LPVOID a)
{
	for (int i = 0; i < 10; i++)
	{

		WaitForSingleObject(DefaultMutex, INFINITE);

		cout << "l";
		Sleep(100);
		cout << "e";
		Sleep(100);
		cout << "z";
		Sleep(100);
		cout << "b\n";
		Sleep(100);
		ReleaseMutex(DefaultMutex);
	}
	return 0;
}