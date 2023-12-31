#include "menu.h"

void printMenu() {
	cout << "\n----------------- Menu -------------------\n";

	cout << "1. Print VBR information\n";
	cout << "2. Print Root Directory Tree infomation\n";
	cout << "3. File traversal (read content)\n";
	cout << "4. Clear screen\n";
	cout << "0. End program.\n";
}

void response_4() {
	system("cls");
	return;
}

void response_3(vector<NTFS_FILE> FilesList, const vector<vector<NTFS_FILE>>& folderTree) {
	cout << "\n----------------- Open File/Folder -------------------\n";
	int tmp;
	cout << "Input Id (stop = -1): ";
	cin >> tmp;
	while (tmp != -1) {
		for (auto i : FilesList)
		{
			if (i.getId() == tmp)
			{
				if (i.isFolder())
				{
					for (auto j : folderTree[tmp])
					{
						j.printFile();
					}
				}
				else
				{
					if (i.isTXT())
					{
						i.printFile_Name();
						i.printFile_Data();
					}
					else
					{
						cout << "Please open by another tools.\n";
					}
				}
				break;
			}
		}
		cout << "Input Id (stop = -1): ";
		cin >> tmp;
	}

}

void response_2(const vector<NTFS_FILE>& folderTree) {
	cout << "\n----------------- Root of folder tree -------------------\n";

	for (auto i : folderTree)
	{
		i.printFile();
	}
}

void fileTraversal(const vector<NTFS_FILE>& folderTree) {
	for (auto i : folderTree)
	{
		i.printFile();
		//i.printFile_Info(); cout << endl;
	}
}

void response_1(BYTE VBR[]) {
	cout << "\n----------------- VBR information -------------------\n";

	cout << "1. Manager Type: " << "NTFS" << endl;

	int sizeOfSector = Hex2Dec(read_offset("0B", 2, VBR));
	cout << "2. Size of Sector (byte): " << sizeOfSector << endl;

	int sectorPerCluster = Hex2Dec(read_offset("0D", 1, VBR));
	cout << "3. Sector per Cluster: " << sectorPerCluster << endl;

	long long totalSectorOfPhysicDrive = Hex2Dec(read_offset("28", 8, VBR));
	cout << "4. Total sector of Physical Drive: " << totalSectorOfPhysicDrive << endl;

	int startClusterOfMFT = Hex2Dec(read_offset("30", 8, VBR));
	long long startSectorOfMFT = (long long)startClusterOfMFT * (long long)sectorPerCluster;
	cout << "5. Start sector of MFT: " << startSectorOfMFT << endl;

	int indexNum = Dec2TwoComp(Hex2Dec(read_offset("40", 1, VBR)));
	int sizeOfMFTEntry = 1;
	for (int i = 1; i <= abs(indexNum); i++)
	{
		sizeOfMFTEntry *= 2;
	}
	cout << "6. Size of MFT entry: " << sizeOfMFTEntry << endl;

}

int getResponse() {
	cout << "Input your choice: ";
	int choice;
	cin >> choice;

	while (choice < 0 || choice > 4)
	{
		cout << "Invalid choice, please input again: ";
		cin >> choice;
	}

	return choice;
}

void operatingMenu(vector<NTFS_FILE> FilesList, const vector<vector<NTFS_FILE>>& folderTree) {
	wstring prefix = L"\\\\.\\";
	wstring temp;
	cout << "Input Volume Name of NTFS (Uppercase): ";
	wcin >> temp;
	temp = prefix + temp + L":";
	LPCWSTR disk = temp.c_str();

	const int SIZE_OF_VBR = 512;
	BYTE VBR[SIZE_OF_VBR];

	ReadSector(disk, (long long)0, VBR, SIZE_OF_VBR);

	printMenu();

	int choice = getResponse();

	while (choice != 0)
	{
		switch (choice) 
		{
			case 1:
				response_1(VBR);
				break;
			case 2:
				response_2(folderTree[5]);
				break;
			case 3:
				response_3(FilesList, folderTree);
				break;
			case 4:
				response_4();
				break;
		}
		printMenu();
		choice = getResponse();
	}
}