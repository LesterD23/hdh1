#pragma once
#include "Utilities.h"

class NTFS_FILE
{
private:
	int _id;
	string _name;
	LPCWSTR _drive;
	int _idParent;
	int _status;
	int _size;

public:
	NTFS_FILE();
	NTFS_FILE(int ID, string NAME, LPCWSTR DRIVE, int IDPARENT, int STATUS, int SIZE);
	~NTFS_FILE();

public:
	void setId(int ID) { _id = ID; }
	void setDrive(LPCWSTR DRIVE) { _drive = DRIVE; }

public:
	int getId() { return _id; }
	string getName() { return _name; }
	int getIdParent() { return _idParent; }
	LPCWSTR getDrive() { return _drive; }
	int getStatus() { return _status; }
	int getSize() { return _size; }

public:
	void getFile(BYTE sectors[]);
	void printFile();
	void printFileName();

public:
	string filename(const BYTE MFT[1024]);
	void data(LPCWSTR drive, long long id);

};


