#include <windows.h>
#include <iostream>
#include <winternl.h>

constexpr const int get_seed() {
	return '0' * -49234 +
		__TIME__[7] * 1 +
		__TIME__[6] * 10 +
		__TIME__[5] * 60 +
		__TIME__[4] * 3600 +
		__TIME__[3] * 36000;
}

constexpr int g_KEY = get_seed() % 0xFF;

// Compile Time JenkinsOneAtATime32Bit Wide Function
constexpr DWORD HashStringDJB2W(const wchar_t* str) {
	wchar_t c = 0;

	unsigned long hash = (unsigned long)g_KEY;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;
	
	return hash;
}

// Compile Time JenkinsOneAtATime32Bit Ascii Function
constexpr DWORD HashStringDJB2A(const char* str) {
	char c = 0;

	unsigned long hash = (unsigned long)g_KEY;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}

#define HashA( API ) HashStringDJB2A((const char *) API )
#define HashW( API ) HashStringDJB2W((const wchar_t*) API )

#define ctHashA( API ) constexpr auto API##_CTA = HashStringDJB2A((const char *) #API )
#define ctHashW( API ) constexpr auto API##_CTW = HashStringDJB2W((const wchar_t*) L#API )

FARPROC CustomGetProcAddress(IN HMODULE hModule, IN DWORD  lpProcName) {
	PBYTE mBytes = (PBYTE)hModule;


	PIMAGE_DOS_HEADER mDosHeader = (PIMAGE_DOS_HEADER)mBytes;

	if (mDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return NULL;
	}

	PIMAGE_NT_HEADERS mNtHeader = (PIMAGE_NT_HEADERS)(mBytes + mDosHeader->e_lfanew);
	if (mNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		return NULL;
	}

	PIMAGE_EXPORT_DIRECTORY mExportDir = (PIMAGE_EXPORT_DIRECTORY)(mBytes + mNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD mFunctionsAddressesArray = (PDWORD)(mBytes + mExportDir->AddressOfFunctions);

	PDWORD mFunctionsNamesArray = (PDWORD)(mBytes + mExportDir->AddressOfNames);

	PWORD mFunctionsOrdinalsArray = (PWORD)(mBytes + mExportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < mExportDir->NumberOfFunctions; i++) {

		CHAR* pFunctionName = (CHAR*)(mBytes + mFunctionsNamesArray[i]);

		PVOID pFunctionAddress = (CHAR*)(mBytes + mFunctionsAddressesArray[mFunctionsOrdinalsArray[i]]);

		if (lpProcName == HashA(pFunctionName))
			return (FARPROC)pFunctionAddress;

	}

	return NULL;
}

typedef int(__stdcall* msgbox) (HWND, LPCSTR, LPCSTR, UINT);

ctHashA(MessageBoxA);
ctHashW(MessageBoxA);

int main(int ac, char** av) {

	HMODULE hModule = LoadLibraryA("USER32.dll");
	msgbox fnMessageBoxA = NULL;
	msgbox lgMessageBoxA = NULL;

		if (hModule != NULL) {
			fnMessageBoxA = reinterpret_cast<msgbox>(CustomGetProcAddress(hModule, MessageBoxA_CTA)); // Using Djb2 Hash into our custom GetProcAddress
			lgMessageBoxA = reinterpret_cast<msgbox>(GetProcAddress(hModule, "MessageBoxA")); // Legit way to retrieve function
		}

		if (fnMessageBoxA != NULL) {
			(*fnMessageBoxA)(NULL, "I'am from hashed", "Hello", MB_OK);
			(*lgMessageBoxA)(NULL, "I'am from legit", "Hello", MB_OK);
		}

	if (hModule != NULL)
		FreeLibrary(hModule);
	
	return 0;
}