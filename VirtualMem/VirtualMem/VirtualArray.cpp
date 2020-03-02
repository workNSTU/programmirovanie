#include "VirtualArray.h"
#include <cstdlib>
#include <ctime>

//  type должен быть степенью 2 и меньше 512 !!!!!
VIRTUAL *vini(long size, int type) {
	char *buffer;
	 
	if (size <= 0 || type <= 0) return NULL;

	// Выравнивание размера массива на границу страницы
	long sizeBuff = size * type;
	if ((sizeBuff % PAGESIZE ) != 0) sizeBuff = (sizeBuff / PAGESIZE + 1) * PAGESIZE;

	buffer = new char[sizeBuff];

	for (int i = 0; i < sizeBuff; i++) {
		buffer[i] = 0;
	}

	// Создаем бинарный файл и открываем на чтение/запись
	FILE* f = fopen("1.dat", "w+");

	if (f == NULL) {
		return NULL;
	}
	else {	
		fwrite(buffer, sizeof(char), sizeBuff, f);
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
	fseek(arr->Fp, 0, SEEK_END);
	fpos_t pos;
	fgetpos(arr->Fp, &pos);
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
			fseek(arr->Fp,  arr->Number[i] * PAGESIZE, SEEK_SET);
			fwrite(&(arr->Page[i*PAGESIZE]) , sizeof(char), PAGESIZE, arr->Fp);
		}
		
		// Загрузка страницы из файла
        fseek(arr->Fp,  page * PAGESIZE, SEEK_SET);
        fread(&(arr->Page[i*PAGESIZE]), sizeof(char), PAGESIZE, arr->Fp);
        arr->Number[i] = page;
		arr->Status[i] &= ~MODIFY_BIT; // сброс флага модификации 
	} 

	// рассчет смещения на странице в байтах
	int offset = ( index % (PAGESIZE / arr->Type) ) * arr->Type;
	return &(arr->Page[i * PAGESIZE + offset]);
}

int vput(VIRTUAL *arr, long index, VTYPE *value) {
	if (addres(arr, index) == NULL) return NULL;
	return fwrite(addres(arr, index), sizeof(char), PAGESIZE, arr->Fp);
}

int vget(VIRTUAL* arr, long index, VTYPE* value) {
	if (addres(arr, index) == NULL) return NULL;
	return fread(addres(arr, index), sizeof(char), PAGESIZE, arr->Fp);
}


