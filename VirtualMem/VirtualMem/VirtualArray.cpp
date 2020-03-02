﻿#include "VirtualArray.h"
#include <cstdlib>
#include <ctime>
#include <windows.h>

VIRTUAL *vini(long size, int type) {
	 
	if (size <= 0 || type <= 0 || ((PAGESIZE % type) != 0)) return NULL;

	// Создаем бинарный файл и открываем на чтение/запись
	FILE* f = fopen("1.dat", "w+");

	if (f == NULL) {
		return NULL;
	}
	else {	

		// Выравнивание размера массива на границу страницы
		long sizeBuff = size * type;
		if ((sizeBuff % PAGESIZE ) != 0) sizeBuff = (sizeBuff / PAGESIZE + 1) * PAGESIZE;

		// Выделение памяти на диске
		if (fseek(f, sizeBuff - 1, SEEK_SET) != 0) return NULL;
		if (!ferror(f)) fputc(0, f);
		else return NULL;

		VIRTUAL *res = new VIRTUAL;
		res->Fp = f;
		res->Type = type;
		for (int i = 0; i < NPAGES; i++) {
			res->Number[i] = i;
			res->Status[i] &= ~MODIFY_BIT; // сброс флага модификации 
		}

		return res;
	}
}

void* addres(VIRTUAL* arr, long index) {
	// номер страницы
	long page = index / (PAGESIZE / arr->Type);

	// Узнаем есть ли страница в массиве
	if (fseek(arr->Fp, 0, SEEK_END) != 0) return NULL;
	fpos_t pos;
	if (fgetpos(arr->Fp, &pos) != 0) return NULL;
	if ( (pos / PAGESIZE) <= page ) return NULL;


	int i = 0;
	while ((i < NPAGES) && (arr->Number[i] != page)) {
		i++;
	}
	
	// Страницы нет в памяти
	if (i >= NPAGES) {

		// Рандомная страница в памяти для замены
		srand(time(0));
		i = rand() % NPAGES;

		// Если бит модификации установлен, то выгружаем страницу
		if ((arr->Status[i] & MODIFY_BIT) > 0) {
			if (fseek(arr->Fp,  arr->Number[i] * PAGESIZE, SEEK_SET) != 0) return NULL;
			if (fwrite(&(arr->Page[i*PAGESIZE]) , sizeof(char), PAGESIZE, arr->Fp) != PAGESIZE) return NULL;
		}
		
		// Загрузка страницы из файла

        if (fseek(arr->Fp,  page * PAGESIZE, SEEK_SET) != 0) return NULL;

		// При ошибке данной операции портится массив и дальнейшая работа с массивом невозможна
		if (fread(&(arr->Page[i*PAGESIZE]), sizeof(char), PAGESIZE, arr->Fp) != PAGESIZE) {
				// исключение
		}

        arr->Number[i] = page;
		arr->Status[i] &= ~MODIFY_BIT; // сброс флага модификации 
	} 

	// рассчет смещения на странице в байтах
	int offset = ( index % (PAGESIZE / arr->Type) ) * arr->Type;
	return &(arr->Page[i * PAGESIZE + offset]);
}

int vput(VIRTUAL *arr, long index, VTYPE *value) {
	void *addr = addres(arr, index);
	if ((addr == NULL) || (addr == value)) return -1;

	// Индекс страницы в массиве STATUS
	int i = ((char*)addr - arr->Page) / PAGESIZE;

	arr->Status[i] |= MODIFY_BIT; // установка флага модификации  

	memcpy(addr, value, arr->Type);
	return 0;
}

int vget(VIRTUAL* arr, long index, VTYPE* value) {
	void *addr = addres(arr, index);
	if ((addr == NULL) || (addr == value)) return -1;
	memcpy(value, addr, arr->Type);
	return 0;
}


