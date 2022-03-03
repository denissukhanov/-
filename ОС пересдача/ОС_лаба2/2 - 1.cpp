#include <iostream>
#include <Windows.h>
using namespace std;

int main() {

	setlocale(LC_ALL, "ru");

	MEMORYSTATUS memoryStatus;

	while ((GetAsyncKeyState(0x51) != -32767)) {
		GlobalMemoryStatus(&memoryStatus);
		cout << "Объем всей памяти: " << (memoryStatus.dwTotalPhys >> 20) << " Мб" << endl;
		cout << "Доступной памяти: " << (memoryStatus.dwAvailPhys >> 20) << " Мб" << endl;
		cout << "Загруженной памяти: " << memoryStatus.dwMemoryLoad << " Мб" << endl;
		cout << "Обновление. . .\n" << endl;
		for (int i = 0; i < 100; i++) {
			if (GetAsyncKeyState(0x51) != -32767) {
				Sleep(10);
			}
			else {
				system("pause");
				return 0;
			}
		}
	}
	return 0;
}