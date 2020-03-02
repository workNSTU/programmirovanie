#include "VirtualArray.h"
#include <iostream>
#include <conio.h>
#include <ctime>

using namespace std;

int main() {
	setlocale(LC_ALL, "RUS");

	VIRTUAL *v = vini(5000, sizeof(int));
	int sw;
	char* test;
	test = new char[NPAGES*PAGESIZE];

	addres(v, 512);

	for (;;)
	{
		cout << "\tМеню\n1 - Сгенерировать и вывести набор тестовых данных\n2 - Ввести тестовые данные в файл\n3 - Чтение из файла\n4 - Выход из программы\n";
		cin >> sw;
		switch (sw) {
		case 1:
			//for (int i = 0; i < NPAGES * PAGESIZE; i++) {
			//	srand(time(0));
			//	test[i] = rand() % 100;
			//	cout << test[i]<<
			//}
			break;
		case 2:
			//vput(v,512,
			break;
		case 3:
			//vget(v,512,
			break;
		case 4:
			fclose(v->Fp);
			return NULL;
		default:
			cout << "Ты дурак!\n";
			break;
		}
	}
}
