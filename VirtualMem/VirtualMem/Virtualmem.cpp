#include "VirtualArray.h"
#include <iostream>
#include <conio.h>
#include <random>


using namespace std;

#define SIZE 1100
typedef char TYPE;

int main() {
	setlocale(LC_ALL, "RUS");

	char *test = new TYPE[SIZE];
	char *end = new TYPE[SIZE];
	VIRTUAL *v = vini(SIZE, sizeof(TYPE));
	int sw;
	
	random_device rd;
	mt19937 mersenne(rd());		//генератор случайных значений

	for (;;)
	{
		cout << "\tМеню\n1 - Сгенерировать и вывести набор тестовых данных\n2 - Ввести тестовые данные в массив\n3 - Чтение из массива\n4 - Выход из программы\n";
		cin >> sw;
		switch (sw) {		//реализация меню
		case 1:
			for (int i = 0; i < SIZE; i++) {		//заполнение массива случайными символами
				srand(mersenne());
				test[i] = TYPE(rand() % 26 + 0x61);
				cout << test[i] << " ";
			}
			break;
		case 2:
			for (int i = 0; i < SIZE; i++) {		//запись в массив
				if (vput(v, i, &(test[i])) == -1) {
					cout << "Произошла ошибка записи в массив";
					break;
				}
			}
			break;
		case 3:
			for (int i = 0; i < SIZE; i++) {	//чтение из массива
				if (vget(v, i, &(end[i])) == -1) {
					cout << "Отказ в чтении элемента";
					break;
				}
				cout << end[i] << ' ';
			}
			break;
		case 4:		//закрытие программы
			saveAllPages(v);
			fclose(v->Fp);
			return 0;
		default:		//exception
			cout << "Неверная команда\n";
			break;
		}
	}
}
