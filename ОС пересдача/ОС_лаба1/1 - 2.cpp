#include <iostream>
#include <windows.h>
#include <tchar.h>
using namespace std;

int main() {
	setlocale(LC_ALL, "ru");

	TCHAR inputFile[20], outputFile[20], bu[500];
	DWORD position, sizeFile, bytesRead, bytesWritten;
	HANDLE f1, f2; //по простому - дескриптор, т.е. число, с помощью которого можно идентифицировать ресурс.

	cout << "Введи имя файла входного файла: ";
	wcin >> inputFile;
	cout << "Введи имя выходного файла: ";
	wcin >> outputFile;
	cout << "Введи позицию для копирования: ";
	cin >> position;
	bool checksBoolValue; //булева для тестов

	f1 = CreateFile(LPCWSTR(inputFile), GENERIC_READ, FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	/*	1 - указатель на строку с именем файла,
		2 - режим доступа к файлу конкретно тут - запись и чтение
		3 - совместный доступ в нашем случае разрешены последующие операции открытия
		объекта, которые требуют доступа к его удалению
		4 - может ли возвращенный дескриптор наследоваться дочерними процессами, сейчес нет
		5 - действия с файлами, сейчас - открытие файла, если файла нет - ошибка
		6 - атрибуты и флажки файла, сейчас - у файла нет других устан атрибутов
		7 - дескриптор файла шаблона с правом доступа G_READ, предоставляет атрибуты файла
	*/
	if (f1 == INVALID_HANDLE_VALUE) {
		cout << "\nВНИМАНИЕ! ОШИБКА ПРИ ИНИЦИАЛИЗАЦИИ ПЕРВОГО ФАЙЛА!" << endl;
		cout << "КОД ОШИБКИ: " << GetLastError << endl;
		CloseHandle(f1);
		return 1;
	}
	else {

		sizeFile = GetFileSize(f1, NULL); //на вход - дескриптор и старшее слово размера файла
		if (sizeFile == INVALID_FILE_SIZE) {
			cout << "\nВНИМАНИЕ! ОШИБКА ПРИ ПОЛУЧЕНИЕ РАЗМЕРА ИСХОДНОГО ФАЙЛА!" << endl;
			cout << "КОД ОШИБКИ: " << GetLastError << endl;
			return 1;
		}

		checksBoolValue = SetFilePointer(f1, (position * sizeof(TCHAR))/2, NULL, FILE_BEGIN);
		// 1 - дескриптор, 2 - байты перемещения
		// 3 - растояние, на которое передвигается (не нужны), 4 - точка отсчета = нулю или началу файла
		if (checksBoolValue == INVALID_SET_FILE_POINTER) {
			cout << "\nВНИМАНИЕ! ОШИБКА В УСТАНОВКЕ УКАЗАТЕЛЯ ПОЗИЦИИ!" << endl;
			cout << "КОД ОШИБКИ: " << GetLastError << endl;
			return 1;
		}

		checksBoolValue = ReadFile(f1, bu, 500, &bytesRead, NULL);
		/*	1 - дескриптор, 2 - указатель на буфер по приему прочитанных данных из файлов,
			3 - число байт из файла для прочтения, 4 - указатель на переменную, с кол-вом прочитанных байт,
			5 - указатель на структуру OVERLAPPED, нам не нужно*/
		if (checksBoolValue == 0) {
			cout << "\nВНИМАНИЕ! ОШИБКА ЧТЕНИЕ ФАЙЛА!" << endl;
			cout << "КОД ОШИБКИ: " << GetLastError << endl;
			return 1;
		}
		if (bytesRead == 0) {
			cout << "\nВНИМАНИЕ! ФАЙЛ ПУСТОЙ ИЛИ ЗАДАНА" << endl;
			cout << "КОД ОШИБКИ: " << GetLastError << endl;
			return 1;
		}
		if ((checksBoolValue != 0) && (bytesRead != 0)) {
			cout << "\nИсходный файл успешно прочитан!" << endl;
		}
		CloseHandle(f1);

		f2 = CreateFile(LPCWSTR(outputFile), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		/* 2 - для записи и чтения, 3 - разрешает послед. операции открытия объекта для записи,
		5 - создает новый файл, либо перезаписывает старый*/

		if (f2 == INVALID_HANDLE_VALUE) {
			cout << "\nВНИМАНИЕ! ОШИБКА ПРИ ИНИЦИАЛИЗАЦИИ ВТОРОГО ФАЙЛА!" << endl;
			cout << "КОД ОШИБКИ: " << GetLastError << endl;
			CloseHandle(f2);
			return 1;
		}
		else {

			SetFilePointer(f2, FILE_BEGIN, NULL, FILE_BEGIN);
			checksBoolValue = WriteFile(f2, &bu, bytesRead, &bytesWritten, NULL);
			/*  1 - дескриптор файла в который запсиываем,
				2 - указатель на буфер с записываемыми данными
				3 - количество байтов на запись,
				4 - указатель на переменную с кол-вом реально записанных байт,
				5 - указатель на стр OVERLAPPED, просто не нужен*/
			if ((checksBoolValue == 0) || bytesWritten != bytesRead) {
				cout << "\nВНИМАНИЕ! ОШИБКА В ЗАПИСИ!" << endl;
				cout << "КОД ОШИБКИ: " << GetLastError << endl;
				return 1;
			}
			else {
				cout << "\nДанные из исходнго файла с позиции " << position << " успешно скопированы в выходной файл" << endl;
				cout << "Исходный файл удален" << endl;
			}
		}
	}
	checksBoolValue = DeleteFile(inputFile);

	system("pause");
	return 0;
}