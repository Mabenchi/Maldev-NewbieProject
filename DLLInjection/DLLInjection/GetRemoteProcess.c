#include "DLLInjection.h"

BOOL GetRemoteProcessHandle(IN LPWSTR szProcessName, OUT DWORD* dwProcessId, OUT HANDLE* hProcess) {
	PROCESSENTRY32 Proc = { 0 };
	LPWSTR lowerStr = NULL;
	LPWSTR szProcessNameLower = toLowerCase(szProcessName);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	*dwProcessId = NULL;
	*hProcess = NULL;

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("[!] CreateToolhelp32Snapshot failed with Error: %d\n", GetLastError());
		goto _Return;
	}

	Proc.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &Proc)) {
		printf("[!] Process32First failed with Error: %d\n", GetLastError());
		goto _Return;
	}

	do {

		lowerStr = toLowerCase(Proc.szExeFile);
		
		if (wcscmp(lowerStr, szProcessNameLower) == 0) {
			*dwProcessId = Proc.th32ProcessID;
			*hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Proc.th32ProcessID);

			if (!*hProcess)
				printf("[!] OpenProcess failed with Error: %d\n", GetLastError());
			free(lowerStr);
			break;
		}
		free(lowerStr);

	} while (Process32Next(hSnapshot, &Proc));

_Return:
	free(szProcessNameLower);
	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
	if (*dwProcessId == NULL || *hProcess == NULL)
		return FALSE;
	return TRUE;
}

wchar_t* toLowerCase(const wchar_t* str) {
	if (str == NULL) return NULL;

	size_t len = wcslen(str);
	wchar_t* lowerStr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));

	if (lowerStr == NULL) return NULL;

	for (size_t i = 0; i < len; ++i) {
		lowerStr[i] = towlower(str[i]);
	}

	lowerStr[len] = L'\0'; 
	return lowerStr; 
}