#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{

	setlocale(LC_ALL, "ru");

	HINSTANCE hlib = LoadLibrary(TEXT("lab6_dll.dll"));

	if (!hLib) {
		cout << GetLastError << endl;
		cout << "ОШИБКА--1" << endl;
		return 1;
	}

	LPTHREAD_START_ROUTINE first = (LPTHREAD_START_ROUTINE)GetProcAddress(hlib, "first");
	LPTHREAD_START_ROUTINE second = (LPTHREAD_START_ROUTINE)GetProcAddress(hlib, "second");

	if (!first || !second) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-0" << endl;
		return 1;
	}

	DefaultMutex = CreateMutex(NULL, FALSE, NULL);

	HANDLE firstAct = CreateThread(NULL, 0, first, NULL, 0, NULL);
	if (firstAct == NULL)
	{
		cout << GetLastError << endl;
		cout << "ОШИБКА-1" << endl;
		return 1;
	}

	HANDLE secondAct = CreateThread(NULL, 0, second, NULL, 0, NULL);
	if (secondAct == NULL)
	{
		cout << GetLastError << endl;
		cout << "ОШИБКА-2" << endl;
		return 1;
	}

	HANDLE Actions[2] = {firstAct, secondAct};
	if (Actions == NULL)
	{
		cout << GetLastError << endl;
		cout << "ОШИБКА-3" << endl;
		return 1;
	}

	DWORD actWaitCheck1 = WaitForSingleObject(Actions[0], 10000);
	if (actWaitCheck1 != WAIT_OBJECT_0)
	{
		cout << GetLastError << endl;
		cout << "ОШИБКА-4" << endl;
		return 1;
	}

	DWORD actWaitCheck2 = WaitForSingleObject(Actions[1], 10000);
	if (actWaitCheck2 != WAIT_OBJECT_0)
	{
		cout << GetLastError << endl;
		cout << "ОШИБКА-5" << endl;
		return 1;
	}

	CloseHandle(DefaultMutex);

	FreeLibrary(hlib);

	system("pause");
	return 0;
}