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
	if (v == NULL) {
		cout << "Массив не может быть создан";
		return -1;
	}

	int sw;
	long startIndex;
	long endIndex;
	
	random_device rd;
	mt19937 mersenne(rd());		//генератор случайных значений

	for (;;)
	{
		cout << "\n\tМеню\n1 - Сгенерировать и вывести набор тестовых данных\n2 - Ввести тестовые данные в массив\n3 - Чтение из массива\n4 - Замена блока\n5 - Выход из программы\n";
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
		case 3:		//чтение из массива
			int swit;
			cout << "Введите номер блока для чтения(0-" << getCountPages(v) - 1 << ")" << endl;
			cin >> swit;
			
			startIndex = (PAGESIZE / sizeof(TYPE)) * swit;
			endIndex = startIndex + (PAGESIZE / sizeof(TYPE)) - 1;

			for (int i = startIndex; i <= endIndex; i++) 
			{
			    if (vget(v, i, &(end[i])) == -1) {
					cout << "Отказ в чтении элемента";
					break;
				}
				cout << end[i] << ' ';
			}
			break;
		case 4:		//замена блок

			cout << "Введите номер блока для замены(0-" << getCountPages(v) - 1 << ")" << endl;
			cin >> swit;

			startIndex = (PAGESIZE / sizeof(TYPE)) * swit;
			endIndex = startIndex + (PAGESIZE / sizeof(TYPE)) - 1;

			for (int i = startIndex; i <= endIndex; i++) {
				char a = 48 + rand() % 10;
				cout << a << " ";
				if (vput(v, i, &a) == -1) {
					cout << "Ошибка записи в массив";
					break;
				}
			}

			break;
		case 5:		//закрытие программы
			saveAllPages(v);
			fclose(v->Fp);
			return 0;
		default:		//exception
			cout << "Неверная команда\n";
			break;
		}
	}
}
