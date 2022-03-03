#include <iostream>
#include <Windows.h>
using namespace std;

//проверка состояния памяти
void check_s(DWORD a) {

	cout << "Memory state is: ";

	switch (a) {
	case MEM_COMMIT: {
		cout << "Commited" << endl;
		break;
	}
	case MEM_RESERVE: {
		cout << "Reserved" << endl;
		break;
	}
	case MEM_DECOMMIT: {
		cout << "Decommited" << endl;
		break;
	}
	case MEM_RELEASE: {
		cout << "Released" << endl;
		break;
	}
	}
}

//проверка состояния страниц
void check_p(DWORD b) {

	cout << "Page protect is: ";

	switch (b) {
	case NULL: {
		cout << "Null" << endl;
		break;
	}
	case PAGE_NOACCESS: {
		cout << "No access" << endl;
		break;
	}
	case PAGE_READONLY: {
		cout << "Readonly" << endl;
		break;
	}
	case PAGE_READWRITE: {
		cout << "Readwrite" << endl;
		break;
	}
	}
}

int main() {

	setlocale(LC_ALL, "ru");
	bool checkErrors;

	int pages;

	//резервируем ._.

	cout << "Введи кол-во страниц виртуальной памяти: ";
	cin >> pages;

	//при успешном выполнение Valloc вернет указатель, иначе - null
	void* pointerV = NULL;

	//размер страницы
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	int size = info.dwPageSize;


	// есть возможность выделить или зарезервировать страницы виртуальной памяти.
	//базовый адресс, размер, соспоб получения, тип доступа
	pointerV = VirtualAlloc(NULL, pages * size, MEM_RESERVE, PAGE_READWRITE);

	if (pointerV != NULL)
		cout << "Успешно зарезервированно. Начальный адресс: " << pointerV << endl;
	else {
		cout << "Ошибка резервации: " << GetLastError();
		return 1;
	}

	//коммитим :/

	//сколько и откуда
	int CommitPages, firstCommitPage;
	cout << "Введи кол-во передаваемых в физ. память страниц: ";
	cin >> CommitPages;
	cout << "Введи номер первое передаваемой страницы: ";
	cin >> firstCommitPage;


	void* pointerVCommit = NULL;

	//необходимый размер
	int needPagesSize = size * CommitPages;

	//резервируем с определенной страницы заданное количество
	pointerVCommit = VirtualAlloc( (pointerV + needPagesSize), CommitPages * size, MEM_COMMIT, PAGE_READWRITE);
	if (pointerVCommit != NULL) {
		cout << "Успешно закоммитили " << CommitPages << " страниц, начиная с " << firstCommitPage << "\nАдресс коммита: " << pointerVCommit << endl;
	}
	else {
		cout << "Ошибка коммита: " << GetLastError();
		return 1;
	}

	//копируем *-*

	int arr[] = { 1,2,3 };

	int* pointerVCommit_first = (int*)pointerVCommit;

	//копирует
	//куда, что, размер
	checkErrors = memcpy(pointerVCommit_first, arr, 3 * sizeof(int));
	if (!checkErrors) {
		cout << "Ошибка memcpy_s: " << GetLastError << endl;
		return 1;
	}
	else {
		cout << "Успешно скопированно!" << endl;
		cout << *(pointerVCommit_first + 0) << ", "
			<< *(pointerVCommit_first + 1) << ", "
			<< *(pointerVCommit_first + 2) << endl;
	}

	//выводим инфу \(-_-)/

	//перед коммитом
	MEMORY_BASIC_INFORMATION mem_info;

	//вывод информации об участке памяти по заданному адресу
	//откуда, стуктура для информации и её размер
	size_t checkQE = VirtualQuery(pointerVCommit, &mem_info, sizeof(mem_info));

	if (checkQE == 0) { 
		cout << "Ошибка вывода информации об участке памяти: " << GetLastError(); 
		return 1; 
	}

	//вывод состояния
	check_s(mem_info.State);
	check_p(mem_info.Protect);

	//после коммита
	cout << "Информация о коммиченной памяти: " << endl;

	checkQE = VirtualQuery((pointerVCommit + needPagesSize), &mem_info, sizeof(mem_info));
	if (checkQE == 0) { 
		cout << "Ошибка вывода информации об участке памяти: " << GetLastError();
		return 1; 
	}

	check_s(mem_info.State);
	check_p(mem_info.Protect);

	//коммитим еще одну страницы +_+

	void* pointerVCommit_second = NULL;
	pointerVCommit_second = VirtualAlloc(pointerV + (firstCommitPage + CommitPages) * size, size, MEM_COMMIT, PAGE_READONLY);
	if (pointerVCommit_second != NULL) {
		cout << "Адрес второй закомм. страницы " << pointerVCommit_second << endl;
	}
	else { 
		cout << "Ошибка второго коммита: " << GetLastError(); 
		return 1; 
	}

	//инфа по второму коммиту :&
	checkQE = VirtualQuery(pointerV + (firstCommitPage + CommitPages) * size, &mem_info, sizeof(mem_info));
	if (checkQE == 0) {
		cout << "Ошибка вывода информации об участке памяти: " << GetLastError();
		return 1; 
	}
	
	check_s(mem_info.State);
	check_p(mem_info.Protect);

	//декоммитим x_x

	SIZE_T de_pages;

	cout << "Сколько страниц вернуть?: ";
	cin >> de_pages;

	cout << "С какой страницы декоммитить?: ";
	cin >> firstCommitPage;

	//освобождаем
	//адресс, размер, действие
	checkQE = VirtualFree(pointerV + firstCommitPage * size, de_pages, MEM_DECOMMIT);
	if (checkQE == 0) {
		cout << "Ошибка в деккомиет страниц: " << GetLastError();
		return 1;
	}
	cout << "Начальный адресс области: " << pointerV + firstCommitPage * size << endl;

	//ну и выводим инфу о том что стало
	checkQE = VirtualQuery(pointerV + firstCommitPage * size, &mem_info, sizeof(mem_info));
	if (checkQE == 0) { 
		cout << "Ошибка вывода информации об участке памяти: " << GetLastError(); 
		return 1; 
	}

	check_s(mem_info.State);
	check_p(mem_info.Protect);

	//последнее освобождение ,-,
	checkQE = VirtualFree(pointerV, 0, MEM_RELEASE);

	if (checkQE == 0) {
		cout << "Ошибка освобождение зарез. памяти: " << GetLastError << endl;
		return 1;
	}

	return 0;
}