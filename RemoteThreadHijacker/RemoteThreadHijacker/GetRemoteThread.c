#include "RemoteThreadHijacking.h"

BOOL GetRemoteThread(IN DWORD dwProcessId, OUT DWORD* dwThreadId, OUT HANDLE* hThread) {
	
	HANDLE			hSnapshot	= NULL;
	THREADENTRY32	Thr		= { 0 };

	Thr.dwSize = sizeof(THREADENTRY32);

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	if (!hSnapshot) {
		printf("{!] CreateToolhelp32Snapshot Failed with an Error: %ld\n", GetLastError());
		goto _Return;
	}


	if (!Thread32First(hSnapshot, &Thr)) {
		printf("{!] Thread32First Failed with an Error: %ld\n", GetLastError());
		goto _Return;
	}

	do {

		if (Thr.th32OwnerProcessID == dwProcessId) {

			*dwThreadId = Thr.th32ThreadID;
			*hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Thr.th32ThreadID);

			if (!hThread) {
				printf("{!] OpenThread Failed with an Error: %ld\n", GetLastError());
				goto _Return;
			}

			break;
		}

	} while (Thread32Next(hSnapshot, &Thr));
_Return:
	if (hSnapshot != NULL)
		CloseHandle(hSnapshot);
	if (*dwThreadId == NULL || *hThread == NULL)
		return FALSE;
	return TRUE;

}