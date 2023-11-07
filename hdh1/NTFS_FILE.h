#pragma once
#include "Utilities.h"

class NTFS_FILE
{
private:
	CONST LPCWSTR DEFAULT_DRIVE = L"\\\\.\\E:";

private:
	int _id;
	wstring _name;
	LPCWSTR _drive;
	int _idParent;
	int _status;
	int _size;
	int _flat;

public:
	NTFS_FILE();
	NTFS_FILE(int ID, wstring NAME, LPCWSTR DRIVE, int IDPARENT, int STATUS, int SIZE, int FLAT);
	~NTFS_FILE();

public:
	void setId(int ID) { _id = ID; }
	void setDrive(LPCWSTR DRIVE) { _drive = DRIVE; }

public:
	int getId() { return _id; }
	wstring getName() { return _name; }
	int getIdParent() { return _idParent; }
	LPCWSTR getDrive() { return _drive; }
	int getStatus() { return _status; }
	int getSize() { return _size; }
	int getFlat() { return _flat; }

public:
	void getFile(BYTE sectors[]);
	void printFile_Info();
	void printFile_Name();
	void printFile_Data();

public:
	wstring findFileName(const BYTE MFT[1024]);
	wstring findData();
	long long realSize(const BYTE MFTEntry[1024]);
	long long data_sector(const BYTE MFTEntry[1024]);

public:
	bool isArchive();
	bool isFolder();
	bool isTXT();
};


