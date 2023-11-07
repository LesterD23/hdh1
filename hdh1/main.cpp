#include "Utilities.h"
#include "NTFS_FILE.h"
#include "NTFS_DISK.h"

int main() {
	cout << "NTFS\n";

// read first part of NTFS disk - VBR
	const int SIZE_OF_VBR = 512;
	const LPCWSTR DEFAULT_DISK = L"\\\\.\\E:";
	BYTE VBR[SIZE_OF_VBR];
	ReadSector(DEFAULT_DISK, (long long) 0, VBR, SIZE_OF_VBR);

// Get main_info in VBR - BPB: 73 bytes
	const int SIZE_OF_SECTOR = Hex2Dec(read_offset("0B", 2, VBR));

	const int SECTORS_PER_CLUSTER = Hex2Dec(read_offset("0D", 1, VBR));

	const int START_CLUSTER_OF_MFT = Hex2Dec(read_offset("30", 8, VBR));
	const long long START_SECTOR_OF_MFT = (long long)START_CLUSTER_OF_MFT * (long long)SECTORS_PER_CLUSTER;

	const int DEFAULT_SIZE_OF_MFTENTRY = 1024; // Hex2Dec(read_offset("40", 1, VBR));

	// const int MAX_READPOINT = 2147483136; // ???

	long long readPoint = START_SECTOR_OF_MFT * (long long)SIZE_OF_SECTOR;

// Read first entry (file) of MFT - $MFT
	BYTE sectorOf$MFT[DEFAULT_SIZE_OF_MFTENTRY];
	ReadSector(DEFAULT_DISK, readPoint, sectorOf$MFT, DEFAULT_SIZE_OF_MFTENTRY);
	readPoint += DEFAULT_SIZE_OF_MFTENTRY;

	NTFS_FILE fileOf$MFT;
	fileOf$MFT.getFile(sectorOf$MFT);

	string readPoint_$MFTAttr = read_offset("14", 2, sectorOf$MFT);
	const int SIZE_OF_MFT = 64 * 8 / 2; //To-do: get MFT size in $MFT

// Get list of MFT entry 
	vector<NTFS_FILE> FilesList;
	FilesList.push_back(fileOf$MFT);

	BYTE MFTentry[DEFAULT_SIZE_OF_MFTENTRY];
	ReadSector(DEFAULT_DISK, readPoint, MFTentry, DEFAULT_SIZE_OF_MFTENTRY);

	int cnt = SIZE_OF_MFT;
	while (--cnt)
	{
		NTFS_FILE tmpFile;
		tmpFile.getFile(MFTentry);
		FilesList.push_back(tmpFile);

		readPoint += 1024;
		ReadSector(DEFAULT_DISK, readPoint, MFTentry, DEFAULT_SIZE_OF_MFTENTRY);
	}

// Print files list
	for (auto& i : FilesList)
	{
		if (i.getName()[0] != '$' && (i.isArchive() || i.isFolder()))
		{
			// i.printFile_Info(); cout << endl;
		}

		if (i.getName()[0] != '$' && (i.isArchive() || i.isFolder()))
		{
			//i.printFile_Name();
			//i.printFile_Data();
		}
	}
	cout << endl;

// Create folder tree
	vector<vector<NTFS_FILE>> folderTree(FilesList.size());
	for (auto& i : FilesList) 
	{
		if (i.getId() != 5 && i.getName()[0] != '$' && (i.isArchive() || i.isFolder()))
		{
			// i.printFile_Info(); cout << endl;
			i.printFile_Name();
			i.printFile_Data();
			folderTree[i.getIdParent()].push_back(i);
		}
	}

// Print root of folder tree
	cout << "Root of folder tree:\n";
	const int ID_OF_ROOT = 5;
	for (auto& i : folderTree[ID_OF_ROOT])
	{
		//i.printFile_Name();
		//i.printFile_Info(); cout << endl;
	}

// Finished
	system("pause");

	return 0;
}

