#pragma once
#include "Utilities.h"
#include "NTFS_FILE.h"


class NTFS_DISK
{
private:
	const LPCWSTR DEFAULT_DISK = L"\\\\.\\E:";
	const int SIZE_OF_VBR = 512;
	//BYTE VBR[SIZE_OF_VBR];

public:
	NTFS_DISK();
	NTFS_DISK(LPWSTR disk);
	~NTFS_DISK();



};


