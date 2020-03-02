#include "VirtualArray.h"
#include <cstdlib>
#include <ctime>

//  type ������ ���� �������� 2 � ������ 512 !!!!!
VIRTUAL *vini(long size, int type) {
	char *buffer;
	 
	if (size <= 0 || type <= 0) return NULL;

	// ������������ ������� ������� �� ������� ��������
	long sizeBuff = size * type;
	if ((sizeBuff % PAGESIZE ) != 0) sizeBuff = (sizeBuff / PAGESIZE + 1) * PAGESIZE;

	buffer = new char[sizeBuff];

	for (int i = 0; i < sizeBuff; i++) {
		buffer[i] = 0;
	}

	// ������� �������� ���� � ��������� �� ������/������
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
			res->Status[i] &= ~MODIFY_BIT; // ����� ����� �����������
		}
		return res;
	}
}

void* addres(VIRTUAL* arr, long index) {
	// ����� ��������
	long page = index / (PAGESIZE / arr->Type);

	// ������ ���� �� �������� � �������
	fseek(arr->Fp, 0, SEEK_END);
	fpos_t pos;
	fgetpos(arr->Fp, &pos);
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
			fseek(arr->Fp,  arr->Number[i] * PAGESIZE, SEEK_SET);
			fwrite(&(arr->Page[i*PAGESIZE]) , sizeof(char), PAGESIZE, arr->Fp);
		}
		
		// �������� �������� �� �����
        fseek(arr->Fp,  page * PAGESIZE, SEEK_SET);
        fread(&(arr->Page[i*PAGESIZE]), sizeof(char), PAGESIZE, arr->Fp);
        arr->Number[i] = page;
		arr->Status[i] &= ~MODIFY_BIT; // ����� ����� ����������� 
	} 

	// ������� �������� �� �������� � ������
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


