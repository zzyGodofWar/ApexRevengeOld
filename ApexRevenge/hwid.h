#pragma once
//#include "sys\HWID-Win10.sys.h"
//#include "sys\HWID-Win7.sys.h"
#include "sys\FakeHwid.sys.h"
#include "main.h"

WCHAR hwiddrvBinaryPath[MAX_PATH];

char* GenerateGUID(char *guidbuffer) {
	for (int i = 0; i < 36; i++) {
		if (i == 8 || i == 13 || i == 18 || i == 23) {
			guidbuffer[i] = '-';
		}
		else {
			int ulValue = rand();
			ulValue = ulValue % 100;
			if (ulValue >= 60) {
				ulValue = rand();
				ulValue = ulValue % (L'9' - L'0' + 1) + L'0';
			}
			else {
				ulValue = rand();
				ulValue = ulValue % ('f' - 'a' + 1) + L'a';
			}
			guidbuffer[i] = (char)ulValue;
		}
	}
	guidbuffer[36] = '\0';
	return guidbuffer;
}

bool SpooferRegister() {
	HKEY mainKey, mainKey1;
	DWORD type = REG_SZ;
	char dwValue[256] = { 0 };

	DWORD dwSize = sizeof(dwValue);

	LSTATUS rstat;
	rstat = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_ALL_ACCESS, &mainKey);
	if (rstat != ERROR_SUCCESS) {
		return false;
	}

	rstat = RegQueryValueExA(mainKey, "guidbck", NULL, &type, (LPBYTE)&dwValue, &dwSize);
	if (rstat != ERROR_SUCCESS) {
		rstat = RegQueryValueExA(mainKey, "MachineGuid", NULL, &type, (LPBYTE)dwValue, &dwSize);
		if (rstat != ERROR_SUCCESS) {
			RegCloseKey(mainKey);
			return false;
		}
		if (RegSetValueExA(mainKey, "guidbck", NULL, type, (LPBYTE)dwValue, dwSize) != ERROR_SUCCESS) {
			RegCloseKey(mainKey);
			return false;
		}
	}

	rstat = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Control\\IDConfigDB\\Hardware Profiles\\0001", 0, KEY_ALL_ACCESS, &mainKey1);
	if (rstat != ERROR_SUCCESS) {
		return false;
	}

	dwSize = sizeof(dwValue);
	rstat = RegQueryValueExA(mainKey1, "guidbck", NULL, &type, (LPBYTE)&dwValue, &dwSize);
	if (rstat != ERROR_SUCCESS) {
		rstat = RegQueryValueExA(mainKey1, "HwProfileGuid", NULL, &type, (LPBYTE)dwValue, &dwSize);
		if (rstat != ERROR_SUCCESS) {
			RegCloseKey(mainKey1);
			return false;
		}
		if (RegSetValueExA(mainKey1, "guidbck", NULL, type, (LPBYTE)dwValue, dwSize) != ERROR_SUCCESS) {
			RegCloseKey(mainKey1);
			return false;
		}
	}

	char uuid[40], hwguid[40];
	srand(time(0));
	GenerateGUID(uuid);
	dwSize = strlen(uuid);
	if (ERROR_SUCCESS != RegSetValueExA(mainKey, "MachineGuid", NULL, type, (LPBYTE)uuid, dwSize)) {
		RegCloseKey(mainKey);
		return false;
	}
	strcpy(hwguid, "{");
	strcat(hwguid, uuid);
	strcat(hwguid, "}");
	dwSize = strlen(hwguid);
	if (ERROR_SUCCESS != RegSetValueExA(mainKey1, "HwProfileGuid", NULL, type, (LPBYTE)hwguid, dwSize)) {
		RegCloseKey(mainKey1);
		return false;
	}
	RegCloseKey(mainKey1);
	RegCloseKey(mainKey);
	return false;
}

bool RestoreRegister() {
	HKEY mainKey;
	DWORD type = REG_SZ;
	char dwValue[256] = { 0 };

	DWORD dwSize = sizeof(dwValue);

	LSTATUS rstat;
	rstat = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_ALL_ACCESS, &mainKey);
	if (rstat != ERROR_SUCCESS) {
		return false;
	}
	rstat = RegQueryValueExA(mainKey, "guidbck", NULL, &type, (LPBYTE)dwValue, &dwSize);
	if (rstat != ERROR_SUCCESS) {
		RegCloseKey(mainKey);
		return false;
	}
	if (RegSetValueExA(mainKey, "MachineGuid", NULL, type, (LPBYTE)dwValue, dwSize) != ERROR_SUCCESS) {
		RegCloseKey(mainKey);
		return false;
	}
	RegCloseKey(mainKey);

	dwSize = sizeof(dwValue);

	rstat = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Control\\IDConfigDB\\Hardware Profiles\\0001", 0, KEY_ALL_ACCESS, &mainKey);
	if (rstat != ERROR_SUCCESS) {
		return false;
	}
	rstat = RegQueryValueExA(mainKey, "guidbck", NULL, &type, (LPBYTE)dwValue, &dwSize);
	if (rstat != ERROR_SUCCESS) {
		RegCloseKey(mainKey);
		return false;
	}
	if (RegSetValueExA(mainKey, "HwProfileGuid", NULL, type, (LPBYTE)dwValue, dwSize) != ERROR_SUCCESS) {
		RegCloseKey(mainKey);
		return false;
	}
	RegCloseKey(mainKey);
	return true;
}


void machinespoofer() {
	VMProtectBegin("HWIDWin");

	SpooferRegister();

	int Strlen = GetTempPath(MAX_PATH, hwiddrvBinaryPath);
	if (Strlen < 1) {
		cout << "¹ý»úÆ÷ÂëÉèÖÃÊ§°Ü\n";
		MessageBoxA(NULL, "»úÆ÷ÂëÎ±×°Ê§°Ü", "×¢Òâ", 0);
		return;
		//cout << "...°´»Ø³µ¼üÍË³ö...";
		//getchar();
		//exit(0);
	}

	WCHAR drv_file_name[16] = { 0 };
	GetRandomString(12, drv_file_name);

	wcscat(hwiddrvBinaryPath, drv_file_name);

	HANDLE file = CreateFile(hwiddrvBinaryPath, GENERIC_ALL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_SYSTEM, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "¹ý»úÆ÷ÂëÉèÖÃÊ§°Ü2\n";
		MessageBoxA(NULL, "»úÆ÷ÂëÎ±×°Ê§°Ü", "×¢Òâ", 0);
		return;
	}

	DWORD sizeWriten, dwSize;
	BYTE* pByte;
	//if (IsWin10System()) {
	//	dwSize = sizeof(hwidbuffer10);
	//	pByte = new BYTE[dwSize + 1];
	//	memcpy(pByte, hwidbuffer10, dwSize);
	//}
	//else {
	//	dwSize = sizeof(hwidbuffer7);
	//	pByte = new BYTE[dwSize + 1];
	//	memcpy(pByte, hwidbuffer7, dwSize);
	//}


	dwSize = sizeof(hwidbuffer);
	pByte = new BYTE[dwSize + 1];
	memcpy(pByte, hwidbuffer, dwSize);

	BYTE *tmpBuf = pByte;
	do {
		WriteFile(file, tmpBuf, dwSize, &sizeWriten, NULL);
		dwSize -= sizeWriten;
		tmpBuf += sizeWriten;

	} while (sizeWriten>0);
	delete[] pByte;

	CloseHandle(file);

	if (!LoadDriver(L"IntelSCSIDisk", L"Intel SCSI Disk Controller", hwiddrvBinaryPath)) {
		cout << "¹ý»úÆ÷ÂëÉèÖÃÊ§°Ü3\n";
		MessageBoxA(NULL, "»úÆ÷ÂëÎ±×°Ê§°Ü", "×¢Òâ", 0);
		return;
	}

	VMProtectEnd();
}