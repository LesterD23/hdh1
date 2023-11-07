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

string read_offset(string offset, int bytesNum, const BYTE sector[]);
string read_offset_raw(string offset, int bytesNum, const BYTE sector[]);
string read_offset_unicode(string offset, int bytesNum, const BYTE sector[]);

wstring clean_wstring(wstring s);
wstring Unicode_Name(string s);
wstring read_offset_wide_string(string offset, int n, const BYTE* sectors);

int Hex2Dec(string hex);
int Dec2TwoComp(int val);

// 
string Dec2Hex(long long decVal);
string MFT_Name(string offset, long long size, const BYTE MFT[1024]);
