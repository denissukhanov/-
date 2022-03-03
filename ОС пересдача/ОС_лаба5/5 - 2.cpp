#include <iostream>
#include <Windows.h>
#include <locale>
using namespace std;


DWORD WINAPI first(LPVOID a) {
	for (int i = 0; i < 10; i++) {
		cout << "E\n"; Sleep(100);
		cout << "V\n"; Sleep(100);
		cout << "A\n"; Sleep(100);
	}
	return 0;
}

DWORD WINAPI second(LPVOID a) {
	for (int i = 0; i < 10; i++) {
		cout << "l"; Sleep(100);
		cout << "e"; Sleep(100);
		cout << "z"; Sleep(100);
		cout << "b"; Sleep(100);
	}
	return 0;
}

int main() {

	setlocale(LC_ALL, "ru");

	//дескр защиты, начальный размер, функция потока, параметр потока, опции создания, id потока
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

	//в  массив кидаем оба потока
	HANDLE Actions[2] = { firstAct, secondAct };
	if (Actions == NULL) {
		cout << GetLastError << endl;
		cout << "ОШИБКА-3" << endl;
		return 1;
	}

	//переход а режим ожидания на 10000мс для каждого потока
	//дескриптор, интервал паузы ожидания
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

	return 0;
}