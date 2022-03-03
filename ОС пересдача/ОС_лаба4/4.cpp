#include <iostream>
#include <locale>
#include <Windows.h>
using namespace std;

int main() {

	setlocale(LC_ALL, "ru");


	bool check;
	char exeName[] = "calc.exe";
	LPSTR cmLine = exeName;

	//структуры с информацией 
	STARTUPINFOA infoStart;
	PROCESS_INFORMATION infoProcess;

	//обнуляем блоки памяти с помощью zeromemory
	ZeroMemory(&infoStart, sizeof(infoStart));


	ZeroMemory(&infoProcess, sizeof(infoProcess));

	//без имени модуля, вызов ком строки, не наслед дескрипторы процесса и потока,
	//наслед дескрт false, без флагов создания, блок конф родителя, старт каталог родителя,
	//стуктура с информ о старте, стукрута с информ о процессе
	check = CreateProcessA(NULL, cmLine, NULL, NULL, CREATE_SUSPENDED, 0, NULL, NULL, &infoStart, &infoProcess);
	if (check == 0) {
		cout << GetLastError << endl;
		return 0;
	}
	else {
		cout << "ID созданного процесса: " << infoProcess.hProcess << endl;
	}

	char choose;
	bool act = true;

	cout << "Выбери: " << endl;
	cout << "s - приостановка потока созданного процесса" << endl;
	cout << "r - возобновление потом процесса" << endl;
	cout << "t - завершение работы процесса" << endl;
	cout << "0 - выход" << endl;


	while (act) {

		cout << "Действие: ";
		cin >> choose;

		switch (choose) {
		case 's': {
			//приостанавливает работу потока
			if (!SuspendThread(infoProcess.hThread)) {
				cout << GetLastError << endl;
				break;
			}
			else {
				cout << "Успешно!" << endl;
				break;
			}
		}
		case 'r': {
			//возобновляет работу потока
			if (!ResumeThread(infoProcess.hThread)) {
				cout << GetLastError << endl;
				break;
			}
			else {
				cout << "Успешно!" << endl;
				break;
			}
		}
		case 't': {
			//завершает работу потока
			if (!TerminateProcess(infoProcess.hProcess, 0)) {
				cout << GetLastError << endl;
				break;
			}
			else {
				cout << "Успешно!" << endl;
				break;
			}
		}
		case '0': {
			act = false;
			break;
		}
		default:
			cout << "ошибка" << endl;
			break;
		}
	}

	
	CloseHandle(infoProcess.hThread);
	CloseHandle(infoProcess.hProcess);
	TerminateProcess(infoProcess.hProcess, 0);
	cout << "Успешно завершено" << endl;

		//блокнот
		char exeName_2[] = "notepad.exe";
	cmLine = exeName_2;

	check = CreateProcessA(NULL, cmLine, NULL, NULL, CREATE_SUSPENDED, 0, NULL, NULL, &infoStart, &infoProcess);
	if (check == 0) {
		cout << GetLastError << endl;
		return 0;
	}
	else {
		cout << "ID созданного процесса: " << infoProcess.hProcess << endl;
	}


	Sleep(10000);
	//продолжает работу
	ResumeThread(infoProcess.hThread);

	//ос использует уровень базового приоритета всех выполняемых потоков, чтобы установить, который поток получает следующий квант процессорного времени.
	cout << "Приоритет: " << GetThreadPriority(infoProcess.hProcess) << endl;


	
	CloseHandle(infoProcess.hThread);
	CloseHandle(infoProcess.hProcess);
	TerminateProcess(infoProcess.hProcess, 0);
	cout << "Успешно завершено" << endl;


		return 0;
}