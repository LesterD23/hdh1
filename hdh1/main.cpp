#include "Utilities.h"
#include "NTFS_FILE.h"

int main() {
	cout << "NTFS\n";

// read first sector of NTFS disk
	const int sizeOfVBR = 512;
	BYTE VBR[sizeOfVBR];
	ReadSector(L"\\\\.\\E:", (long long) 0, VBR, sizeOfVBR);

// Get neccessary info in VBR - BPB
	const int sectorSize = Hex2Dec(read_offset("0B", 2, VBR));
	// const int startSectorOfLogic_Disk = Hex2Dec(read_offset("1C", 4, VBR_Sector1));
	const int sectorPerCluster = Hex2Dec(read_offset("0D", 1, VBR));;
	const int startClusterOfMFT = Hex2Dec(read_offset("30", 8, VBR));
	// const int sizeOfMFT_entry = pow(2, abs(Dec2TwoComp(Hex2Dec(read_offset("40", 1, VBR_Sector1)))));
	// const int startSectorOfMFT = startSectorOfLogic_Disk + startClusterOfMFT * sectorPerCluster;
	const int startSectorOfMFT = startClusterOfMFT * sectorPerCluster;

// Special value
	const int sizeOfMFTentry = 1024;
	const int maxReadPoint = 2147483136; // ???
	long long readPoint = (long long)startSectorOfMFT * (long long)sectorSize;

// get list of NFT entry
	vector<NTFS_FILE> FilesList;
	BYTE MFTentry[sizeOfMFTentry];
	ReadSector(L"\\\\.\\E:", readPoint, MFTentry, sizeOfMFTentry);

	int cnt = 64 * 8 / 2;
	while (cnt--/*read_offset_unicode("00", 4, MFTentry) == "FILE" || read_offset_unicode("00", 4, MFTentry) == "BAAD"*/)
	{
		NTFS_FILE tmp;
		tmp.getFile(MFTentry);
		tmp.setDrive(L"\\\\.\\E:");
		FilesList.push_back(tmp);

		readPoint += 1024;
		ReadSector(L"\\\\.\\E:", readPoint, MFTentry, sizeOfMFTentry);

	}

// print files list
	for (auto& i : FilesList)
	{
		//i.printFile();
		i.printFileName();
		if (i.getName()[0] == '$' || i.getStatus() == 0 || i.getStatus() == 2)
			continue;
		i.data(L"\\\\.\\E:", i.getId());
	}
	cout << endl;

// Finished
	system("pause");

	return 0;
}

