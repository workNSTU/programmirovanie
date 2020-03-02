#ifndef VIRTUAL_ARRAY
#define VIRTUAL_ARRAY

#include <stdio.h>

#define NPAGES 2

// размер страницы в битах
#define PAGESIZE 512

typedef char VTYPE;

// маска бита модификации у элемента массива Status
#define MODIFY_BIT 1

struct VIRTUAL {
	FILE *Fp;
	int Type;
	int Status[NPAGES];
	int Number[NPAGES];
	char Page[NPAGES * PAGESIZE];
};

VIRTUAL *vini(long size, int Type);		//процедура инициализации виртуального массива
int vput(VIRTUAL *arr, long index, VTYPE *value);		//процедура записи в массив
int vget(VIRTUAL *arr, long index, VTYPE *value);		//процедура чтения из массива
void *addres(VIRTUAL *arr, long index);		//процедура вычисления адреса элемента

#endif /* VIRTUAL_ARRAY */