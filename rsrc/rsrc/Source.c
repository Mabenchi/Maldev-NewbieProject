#include <Windows.h>
#include <stdio.h>
#include "resource.h"

int main() {

	HRSRC	hRsrc			= NULL;
	HGLOBAL	hGlobal			= NULL;
	PVOID	pPayloadAddress = NULL;
	SIZE_T	sPayloadSize	= 0;
	PVOID	pTmpRsrc		= NULL;

	hRsrc = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_RCDATA1), RT_RCDATA);
	if (!hRsrc) {
		printf("[!] FindResourceW Failed With Error : %d \n", GetLastError());
		return 1;
	}

	hGlobal = LoadResource(NULL, hRsrc);
	if (!hGlobal) {
		printf("[!] LoadResource Failed With Error : %d \n", GetLastError());
		return 1;
	}

	pPayloadAddress = LockResource(hGlobal);
	if (!pPayloadAddress) {
		printf("[!] LockResource Failed With Error : %d \n", GetLastError());
		return 1;
	}

	sPayloadSize = SizeofResource(NULL, hRsrc);
	if (!sPayloadSize) {
		printf("[!] SizeofResource Failed With Error : %d \n", GetLastError());
		return 1;
	}

	printf("[i] pPayloadAddress\tvar:\t0x%p.\n", pPayloadAddress);
	printf("[i] sPayloadSize\tvar:\t%ld.\n", sPayloadSize);

	pTmpRsrc = HeapAlloc(GetProcessHeap(), NULL, sPayloadSize);
	if (!pTmpRsrc) {
		printf("[!] HeapAlloc Failed With Error : %d \n", GetLastError());
		return 1;
	}

	memcpy(pTmpRsrc, pPayloadAddress, sPayloadSize);

	printf("[i] pTmpRsrc\tvar:\t0x%p.\n", pTmpRsrc);
	printf("[#] Press <Enter> To Continue ...");
	getchar();
	return 0;
}