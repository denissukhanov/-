#include <iostream>
#include <Windows.h>
//инициализация переменной-объекта крит секции
CRITICAL_SECTION CritSect;
using namespace std;

DWORD WINAPI first(LPVOID aaaa) {

	for (int i = 0; i < 10; i++) {

		//вход в крит секцию
		// ждет монопольное использование указанного объекта критической секции
		//она защищает исполняемый поток
		EnterCriticalSection(&CritSect);

		cout << "E\n"; Sleep(100);
		cout << "V\n"; Sleep(100);
		cout << "A\n"; Sleep(100);
		//выход
		LeaveCriticalSection(&CritSect);
		Sleep(1); //квант времени для перехода к след секции
	}
	return 0;
}

DWORD WINAPI second(LPVOID aaaa) {

	for (int i = 0; i < 10; i++) {

		//вход в крит секцию
		EnterCriticalSection(&CritSect);

		cout << "l"; Sleep(100);
		cout << "e"; Sleep(100);
		cout << "z"; Sleep(100);
		cout << "b\n"; Sleep(100);

		//выход
		LeaveCriticalSection(&CritSect);
		Sleep(1); //квант времени для задержки и перехода
	}
	return 0;
}


int main()
{
	setlocale(LC_ALL, "ru");
	//Критическая секция (Critical Section) это участок кода, в котором поток (thread) получает доступ к ресурсу (например переменная), который доступен из других потоков.
	//инициализация крит секции
	InitializeCriticalSection(&CritSect);

	HANDLE firstAct = CreateThread(NULL, 0, first, NULL, 0, NULL);
	if (firstAct == NULL) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-1" << endl;
		return 1;
	}


	HANDLE secondAct = CreateThread(NULL, 0, second, NULL, 0, NULL);
	if (secondAct == NULL) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-2" << endl;
		return 1;
	}

	HANDLE Actions[2] = { firstAct, secondAct };
	if (Actions == NULL) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-3" << endl;
		return 1;
	}

	DWORD actWaitCheck1 = WaitForSingleObject(Actions[0], 10000);
	if (actWaitCheck1 != WAIT_OBJECT_0) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-4" << endl;
		return 1;
	}

	DWORD actWaitCheck2 = WaitForSingleObject(Actions[1], 10000);
	if (actWaitCheck2 != WAIT_OBJECT_0) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-5" << endl;
		return 1;
	}

	//удаление крит секции
	DeleteCriticalSection(&CritSect);

	return 0;
}