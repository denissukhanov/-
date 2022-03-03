#define _CRT_SECURE_NO_WARNINGS
#include "lab6_dll.h"

using namespace std;

int main()
{

	setlocale(LC_ALL, "ru");

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

	system("pause");
	return 0;
}