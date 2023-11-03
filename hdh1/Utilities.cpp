#include "Utilities.h"

int ReadSector(LPCWSTR drive, // LPCWSTR: Long Pointer to Constant Wide String
	long long readPoint, BYTE sectors[], int bytesNum) {

	DWORD bytesRead;
	HANDLE device = NULL;

	device = CreateFileW(drive,    // Drive to open
		GENERIC_READ,           // Access mode
		FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
		NULL,                   // Security Descriptor
		OPEN_EXISTING,          // How to create
		0,                      // File attributes
		NULL);                  // Handle to template

	if (device == INVALID_HANDLE_VALUE) // Open Error
	{
		printf("CreateFile: %u\n", GetLastError());
		return 1;
	}

	int quotient = readPoint / 2147483136;
	int remainder = readPoint % 2147483136;

	if (SetFilePointer(device, remainder, NULL, FILE_BEGIN) == -1) {// Set a Point to Read
		printf("ReadFile: %u\n", GetLastError());
	}

	for (int i = 0; i < quotient; i++) {
		if (SetFilePointer(device, 2147483136, NULL, FILE_CURRENT) == -1) {
			printf("ReadFile: %u\n", GetLastError());
		}
	}

	if (!ReadFile(device, sectors, bytesNum, &bytesRead, NULL))
	{
		printf("ReadFile: %u\n", GetLastError());
	}
	else
	{
		// printf("Success!\n");
	}

}

void PrintSector(const BYTE sectors[], int bytesNum) {
	for (int i = 0; i < bytesNum; i++)
	{
		cout << hex << setw(2) << setfill('0') << int(sectors[i]) << " ";

		if (i % 4 == 3)
			cout << " ";

		if (i % 16 == 15)
			cout << endl;
	}
}

string read_offset(string offset, int bytesNum, const BYTE sector[]) {

	stringstream builder;

	int start_index = stoi(offset, 0, 16);
	int end_index = start_index + (bytesNum - 1);

	builder.clear();

	for (int i = 0; i < bytesNum; i++) {

		builder << hex << setw(2) << setfill('0') << int(sector[end_index - i]);
	}

	return builder.str();
}

string read_offset_raw(string offset, int bytesNum, const BYTE sector[]) {
	stringstream builder;
	int start_index = stoi(offset, 0, 16);

	for (int i = 0; i < bytesNum; i++) {
		builder << hex << setw(2) << setfill('0') << int(sector[start_index + i]);
	}

	return builder.str();
}

string read_offset_unicode(string offset, int bytesNum, const BYTE sector[]) {
	stringstream builder;

	for (int i = 0; i < bytesNum; i++) {

		builder << sector[i];
	}

	return builder.str();
}

int Hex2Dec(string hex) {

	int res = 0;
	int base = 1;

	for (int i = hex.size() - 1; i >= 0; i--) {

		if (hex[i] >= '0' && hex[i] <= '9') {

			res += (int(hex[i]) - 48) * base;
			base *= 16;
		}
		else {

			res += (int(hex[i]) - 87) * base;
			base *= 16;
		}
	}
	return res;
}

int Dec2TwoComp(int val) {
	int res = 0;
	int base = 1;
	for (int i = 0; i < 8; i++)
	{
		int tmp = (val >> i) & 1;
		if (i == 7)
		{
			res -= tmp * base;
		}
		else
		{
			res += tmp * base;
		}
		base *= 2;
	}

	return res;
}

string Dec2Hex(long long decVal) {
	if (decVal == 0)
		return "0";
	string ans;
	int rem = 0;
	while (decVal > 0)
	{
		char ch;
		rem = decVal % 16;
		if (rem < 10) {
			rem += 48;
			ch = (char)rem;
		}
		else {
			rem += 87;
			ch = (char)rem;
		}
		ans += ch;
		decVal /= 16;
	}
	reverse(ans.begin(), ans.end());
	return ans;
}

string MFT_Name(string offset, long long size, const BYTE MFT[1024]) {
	stringstream builder;
	int start_index = stoi(offset, 0, 16);

	for (int i = 0; i < size; i++) {
		builder << MFT[start_index + i];
	}

	return builder.str();
}
