#include "VirtualArray.h"
#include <cstdlib>
#include <ctime>
#include <windows.h>

VIRTUAL *vini(long size, int type) {
	 
	if (size <= 0 || type <= 0 || ((PAGESIZE % type) != 0)) return NULL;

	// ������� �������� ���� � ��������� �� ������/������
	FILE* f = fopen("1.dat", "w+");

	if (f == NULL) {
		return NULL;
	}
	else {	

		// ������������ ������� ������� �� ������� ��������
		long sizeBuff = size * type;
		if ((sizeBuff % PAGESIZE ) != 0) sizeBuff = (sizeBuff / PAGESIZE + 1) * PAGESIZE;

		// ��������� ������ �� �����
		if (fseek(f, sizeBuff - 1, SEEK_SET) != 0) return NULL;
		if (!ferror(f)) fputc(0, f);
		else return NULL;

		VIRTUAL *res = new VIRTUAL;
		res->Fp = f;
		res->Type = type;
		for (int i = 0; i < NPAGES; i++) {
			res->Number[i] = i;
			res->Status[i] &= ~MODIFY_BIT; // ����� ����� ����������� 
		}

		return res;
	}
}

void* addres(VIRTUAL* arr, long index) {
	// ����� ��������
	long page = index / (PAGESIZE / arr->Type);

	// ������ ���� �� �������� � �������
	if (fseek(arr->Fp, 0, SEEK_END) != 0) return NULL;
	fpos_t pos;
	if (fgetpos(arr->Fp, &pos) != 0) return NULL;
	if ( (pos / PAGESIZE) <= page ) return NULL;


	int i = 0;
	while ((i < NPAGES) && (arr->Number[i] != page)) {
		i++;
	}
	
	// �������� ��� � ������
	if (i >= NPAGES) {

		// ��������� �������� � ������ ��� ������
		srand(time(0));
		i = rand() % NPAGES;

		// ���� ��� ����������� ����������, �� ��������� ��������
		if ((arr->Status[i] & MODIFY_BIT) > 0) {
			if (fseek(arr->Fp,  arr->Number[i] * PAGESIZE, SEEK_SET) != 0) return NULL;
			if (fwrite(&(arr->Page[i*PAGESIZE]) , sizeof(char), PAGESIZE, arr->Fp) != PAGESIZE) return NULL;
		}
		
		// �������� �������� �� �����

        if (fseek(arr->Fp,  page * PAGESIZE, SEEK_SET) != 0) return NULL;

		// ��� ������ ������ �������� �������� ������ � ���������� ������ � �������� ����������
		if (fread(&(arr->Page[i*PAGESIZE]), sizeof(char), PAGESIZE, arr->Fp) != PAGESIZE) {
				// ����������
		}

        arr->Number[i] = page;
		arr->Status[i] &= ~MODIFY_BIT; // ����� ����� ����������� 
	} 

	// ������� �������� �� �������� � ������
	int offset = ( index % (PAGESIZE / arr->Type) ) * arr->Type;
	return &(arr->Page[i * PAGESIZE + offset]);
}

int vput(VIRTUAL *arr, long index, VTYPE *value) {
	void *addr = addres(arr, index);
	if ((addr == NULL) || (addr == value)) return -1;

	// ������ �������� � ������� STATUS
	int i = ((char*)addr - arr->Page) / PAGESIZE;

	arr->Status[i] |= MODIFY_BIT; // ��������� ����� �����������  

	memcpy(addr, value, arr->Type);
	return 0;
}

int vget(VIRTUAL* arr, long index, VTYPE* value) {
	void *addr = addres(arr, index);
	if ((addr == NULL) || (addr == value)) return -1;
	memcpy(value, addr, arr->Type);
	return 0;
}


