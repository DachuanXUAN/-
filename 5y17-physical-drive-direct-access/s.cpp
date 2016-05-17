#include <windows.h>
#include <stdio.h>
#include <string.h>

short ReadSect
(const char *_dsk,    // disk to access
	char *&_buff,         // buffer where sector will be stored
	unsigned int _nsect   // sector number, starting with 0
)
{
	DWORD dwRead;
	HANDLE hDisk = CreateFile(_dsk, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hDisk == INVALID_HANDLE_VALUE) // this may happen if another program is already reading from disk
	{
		printf("not valid!\n");
		CloseHandle(hDisk);
		return 1;
	}
	SetFilePointer(hDisk, _nsect * 512, 0, FILE_BEGIN); // which sector to read

	ReadFile(hDisk, _buff, 512, &dwRead, 0);  // read sector
	CloseHandle(hDisk);
	return 0;
}

int main(int n,char ** args)
{
	if (n < 3) return 1;
	const char * drv = "\\\\.\\C:";
	const char *dsk_ = "\\\\.\\PhysicalDrive1";
	char dsk[20];
	strcpy(dsk, dsk_);
	dsk[17] = args[1][0];
	int sector = atoi(args[2]);
	//scanf("%c%d", dsk + 17, &sector);
	char *buff = new char[512];
	ReadSect(dsk, buff, sector);

	const char * fmt16 = "%02X ";
	const char * fmt10 = "%d ";
	const char * fmt = fmt10;
	if (n > 3) fmt = fmt16;
	if ((unsigned char)buff[510] == 0x55 && (unsigned char)buff[511] == 0xaa) 
		printf("Disk is bootable!\n");
	int t;
	for (int i = 0; i < 512; i++) {
		t = buff[i] & 0xFF;
		printf(fmt, t);
		if(i%32==31)
			printf("\n");
	}
	return 0;
}
