#pragma once
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

// Prototype
int ReadSector(LPCWSTR drive, long long readPoint, BYTE sectors[], int bytesNum);
void PrintSector(const BYTE sectors[], int bytesNum);

string read_offset(string offset, int bytesNum, const BYTE sector[512]);
string read_offset_raw(string offset, int bytesNum, const BYTE sector[512]);
string read_offset_unicode(string offset, int bytesNum, const BYTE sector[]);

int Hex2Dec(string hex);
int Dec2TwoComp(int val);

// 
string Dec2Hex(long long decVal);
string read_offset_MFT(string offset, int n, const BYTE MFT[1024]);
string MFT_Name(string offset, long long size, const BYTE MFT[1024]);
