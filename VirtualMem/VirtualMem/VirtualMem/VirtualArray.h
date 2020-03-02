#ifndef VIRTUAL_ARRAY
#define VIRTUAL_ARRAY

#include <stdio.h>

#define NPAGES 2

// ������ �������� � �����
#define PAGESIZE 512

typedef char VTYPE;

// ����� ���� ����������� � �������� ������� Status
#define MODIFY_BIT 1

struct VIRTUAL {
	FILE *Fp;
	int Type;
	int Status[NPAGES];
	int Number[NPAGES];
	char Page[NPAGES * PAGESIZE];
};

VIRTUAL *vini(long size, int Type);		//��������� ������������� ������������ �������
int vput(VIRTUAL *arr, long index, VTYPE *value);		//��������� ������ � ������
int vget(VIRTUAL *arr, long index, VTYPE *value);		//��������� ������ �� �������
void *addres(VIRTUAL *arr, long index);		//��������� ���������� ������ ��������

#endif /* VIRTUAL_ARRAY */