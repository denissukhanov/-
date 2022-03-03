#include <iostream>
#include <Windows.h>
#include <ctime>
using namespace std;

void print(int cols, int* arr[], HANDLE hp) {
	for (int i = 0; i < cols; i++) {

		//heapsize считает размер  блока памяти в куче и возвращает размер в байтах, поэтому перевожим в инт
		//дексриптор, флаг, указатель на памяти
		int row = (int)HeapSize(hp, 0, arr[i]) / sizeof(int);

		for (int j = 0; j < row; j++) {
			cout << *(arr[i] + j) << " ";
		}
		cout << endl;
	}
}

void sort(int cols, int* arr[], HANDLE hp) {

	bool check = true;
	
	//меняет строки местами пока строка больше последующей, обычная сортировочка массива строк внутри другого массива
	while (check) {

		check = false;
		for (int i = 0; i < cols - 1; i++) {

			//сравнивается размер памяти под строки - текущую и следующую
			if (HeapSize(hp, 0, arr[i]) > HeapSize(hp, 0, arr[i + 1])) {
				int* a = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = a;
				check = true;
			}
		}
	}
}

int** create(int cols, HANDLE hp) {

	//создаем обыкновенный двумерный массив через сайзофы
	int row;

	//heapalloc выделяем памяти под кучу с типом инт
	//указатель на кучу, флаги, объем памяти
	int** arr = (int**)HeapAlloc(hp, 0, (cols * sizeof(int*)));

	for (int i = 0; i < cols; i++) {

		//длина каждой строки - рандом от 1 до 10
		row = rand() % 10 + 1;

		//под каждую строчку память тоже, размером с длину строки
		arr[i] = (int*)HeapAlloc(hp, 0, (row * sizeof(int)));

		//вдруг не получилось
		if (arr[i] == NULL) {
			cout << GetLastError() << endl;
			return 0;
		}
		else {
			for (int j = 0; j < row; j++) {

				//заполняем рандомом от 1 до 10
				*(arr[i] + j) = rand() % 10 + 1;
			}
		}
	}

	return arr;
}

int main() {

	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	
	//дексриптор кучи памяти
	HANDLE heap = NULL;
	int cols;

	//heapcreate создает кучу
	//атрибуты, начальный размер, конечный размер
	heap = HeapCreate(0, 0x1000, 0);

	if (heap == NULL) {
		cout << GetLastError() << endl;
		return 1;
	}
	else {

		cout << "Количество строк массива (>5): ";
		cin >> cols;
		while (cols < 5) {
			cout << "БОЛЬШЕ ПЯТИ!: ";
			cin >> cols;
		}
		
		//все раскидано по функциям
		int** arr = create(cols, heap);
		if (arr == 0) {
			return 1;
		}

		cout << "Получилось: " << endl;
		print(cols, arr, heap);

		cout << "Сортируем. . ." << endl;
		sort(cols, arr, heap);

		cout << "Получилось: " << endl;
		print(cols, arr, heap);

		cout << "Очитска кучи. . ." << endl;

		//heapdestroy - разрушает кучу
		//указатель на кучу
		if (HeapDestroy(heap) == 0) {
			cout << GetLastError << endl;
			return 1;
		}
	}

	return 0;
}