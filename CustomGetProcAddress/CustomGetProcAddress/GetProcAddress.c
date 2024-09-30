#include <windows.h>
#include <stdio.h>
#include <winternl.h>


FARPROC CustomGetProcAddress(IN HMODULE hModule, IN LPCSTR  lpProcName) {
	PBYTE mBytes = (PBYTE)hModule;


	PIMAGE_DOS_HEADER mDosHeader = (PIMAGE_DOS_HEADER)mBytes;

	if (mDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return NULL;
	}

	PIMAGE_NT_HEADERS mNtHeader = (PIMAGE_NT_HEADERS)(mBytes + mDosHeader->e_lfanew);
	if (mNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		return NULL;
	}

	PIMAGE_EXPORT_DIRECTORY mExportDir  = (PIMAGE_EXPORT_DIRECTORY) (mBytes + mNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD mFunctionsAddressesArray = (PDWORD)(mBytes + mExportDir->AddressOfFunctions);

	PDWORD mFunctionsNamesArray = (PDWORD)(mBytes + mExportDir->AddressOfNames);

	PWORD mFunctionsOrdinalsArray = (PDWORD)(mBytes + mExportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < mExportDir->NumberOfFunctions; i++) {

		CHAR* pFunctionName = (CHAR*)(mBytes + mFunctionsNamesArray[i]);

		PVOID pFunctionAddress = (CHAR*)(mBytes + mFunctionsAddressesArray[mFunctionsOrdinalsArray[i]]);

		if (strcmp(lpProcName, pFunctionName) == 0)
			return pFunctionAddress;

	}

	return NULL;
}

HMODULE CustomGetModuleHandle(IN LPCWSTR szModuleName) {

#ifdef _WIN64
	PPEB pPeb = (PEB*)(__readgsqword(0x60));
#elif _WIN32
	PPEB pPeb = (PEB*)(__readgsqword(0x30));
#endif

	PLDR_DATA_TABLE_ENTRY	pDte = (PLDR_DATA_TABLE_ENTRY)(pPeb->Ldr->InMemoryOrderModuleList.Flink);


	while (pDte) {
		if (!(pDte->FullDllName.Length))
			break;

		if (lstrcmpiW(pDte->FullDllName.Buffer, szModuleName) == 0) {
#ifdef STRUCTS
			return (HMODULE)(pDte->InInitializationOrderLinks.Flink);
#else
			return (HMODULE)pDte->Reserved2[0];
#endif
		}

		pDte = *(PLDR_DATA_TABLE_ENTRY*)(pDte);
	}

	return NULL;

}



int main() {

	printf("[+] Original GetProcAddress \t: 0x%p \n", GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtAllocateVirtualMemory"));
	printf("[+] GetProcAddress Replacement \t: 0x%p \n", CustomGetProcAddress(CustomGetModuleHandle(L"ntdll.dll"), "NtAllocateVirtualMemory"));

	//CustomGetModuleHandle(L"NTDLL.DLL");

	return 0;

}
