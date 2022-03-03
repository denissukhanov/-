#include <iostream>
#include <Windows.h>
using namespace std;


DWORD WINAPI first(LPVOID a) {
	for (int i = 0; i < 15; i++) {
		cout << "Successful - 1\n";
	}
	return 0;
}

DWORD WINAPI second(LPVOID a) {
	for (int i = 0; i < 15; i++) {
		cout << "Successful - 2\n";
	}
	return 0;
}

int main() {

	//дескр защиты, начальный размер, функция потока, параметр потока, опции создания, id потока
	HANDLE firstAct = CreateThread(NULL, 0, first, NULL, 0, NULL);
	if (firstAct == NULL) {
		cout << GetLastError << endl;
		return 1;
	}


	HANDLE secondAct = CreateThread(NULL, 0, second, NULL, 0, NULL);
	if (secondAct == NULL) {
		cout << GetLastError << endl;
		return 1;
	}
	
	//оба потока - в массив
	HANDLE Actions[2] = { firstAct, secondAct };
	if (Actions == NULL) {
		cout << GetLastError << endl;
		return 1;
	}

	//переход а режим ожидания на 10000мс для каждого потока
//дескриптор, интервал паузы ожидания
	DWORD actWaitCheck1 = WaitForSingleObject(Actions[0], 10000);
	if (actWaitCheck1 != WAIT_OBJECT_0) {
		cout << GetLastError << endl;
		return 1;
	}

	DWORD actWaitCheck2 = WaitForSingleObject(Actions[1], 10000);
	if (actWaitCheck2 != WAIT_OBJECT_0) {
		cout << GetLastError << endl;
		return 1;
	}

	return 0;
}