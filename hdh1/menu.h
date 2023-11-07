#pragma once
#include "Utilities.h"
#include "NTFS_FILE.h"

void printMenu();

void response_4();
void response_3(vector<NTFS_FILE> FilesList, const vector<vector<NTFS_FILE>>& folderTree);
void response_2(const vector<vector<NTFS_FILE>>& folderTree);
void response_1(BYTE VBR[]);

LPCWSTR inputVolumeName();
int getResponse();

void operatingMenu(vector<NTFS_FILE> FilesList, BYTE VBR[], const vector<vector<NTFS_FILE>>& folderTree);