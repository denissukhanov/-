#include <iostream>
#include <Windows.h>
#include <locale>
using namespace std;
//объявление дескриптора переменной
//Данный объект синхронизации регистрирует доступ к ресурсам и может находиться в двух состояниях:
//сброшен или установлен
HANDLE DefaultMutex;

//тут у нас взаимоисключение - тема дня
//Mutex позволяет проводить синхронизацию между потоками(thread) и процессами(process)

DWORD WINAPI first(LPVOID a) {
	for (int i = 0; i < 10; i++) {

		//деск процесса, кот. перевод в режим ожидания, интервал паузы
		WaitForSingleObject(DefaultMutex, INFINITE);

		cout << "E\n"; Sleep(100);
		cout << "V\n"; Sleep(100);
		cout << "A\n"; Sleep(100);

		//освобождение объекта
		ReleaseMutex(DefaultMutex);
	}
	return 0;
}

DWORD WINAPI second(LPVOID a) {
	for (int i = 0; i < 10; i++) {

		WaitForSingleObject(DefaultMutex, INFINITE);

		cout << "l"; Sleep(100);
		cout << "e"; Sleep(100);
		cout << "z"; Sleep(100);
		cout << "b\n"; Sleep(100);

		//освобождение объекта
		ReleaseMutex(DefaultMutex);
	}
	return 0;
}

int main() {

	setlocale(LC_ALL, "ru");

	//создание Mutex 
	//атриб безоп, флаг начального владельца, имя объекта
	DefaultMutex = CreateMutex(NULL, FALSE, NULL);

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

	//закрываем дескриптор по оконачанию
	CloseHandle(DefaultMutex);

	system("pause");
	return 0;
}