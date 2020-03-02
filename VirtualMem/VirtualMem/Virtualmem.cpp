#include "VirtualArray.h"
#include <iostream>
#include <conio.h>
#include <random>


using namespace std;

int main() {
	setlocale(LC_ALL, "RUS");

	VIRTUAL *v = vini(8000, sizeof(char));
	int sw;
	char *test, *end;
	test = new char[8000];		//изначальный массив
	end = new char[8000];		//конечный массив

	addres(v, 8000);
	random_device rd;
	mt19937 mersenne(rd());		//генератор случайных значений

	for (;;)
	{
		cout << "\tМеню\n1 - Сгенерировать и вывести набор тестовых данных\n2 - Ввести тестовые данные в файл\n3 - Чтение из файла\n4 - Выход из программы\n";
		cin >> sw;
		switch (sw) {		//реализация меню
		case 1:
			for (int i = 0; i < 8000; i++) {		//заполнение массива случайными символами
				srand(mersenne());
				test[i] = char(rand() % 26 + 0x61);
				cout << test[i] << " ";
			}
			break;
		case 2:
			for (int i = 0; i < 8000; i++) {		//запись в файл
				vput(v, i, test + i);
			}
			if (vput(v, 0, test + 0) != -1) {
				cout << "Выполнено\n";
			}
			else {
				cout << "Не выполнено\n";
			}
			break;
		case 3:
			for (int i = 0; i < 8000; i++) {	//чтение из файла
				vget(v, i, end + i);
				cout << *(end + i) << ' ';
			}
			break;
		case 4:		//закрытие программы
			fclose(v->Fp);
			return NULL;
		default:		//exception
			cout << "Ошибка!\n";
			break;
		}
	}
}
