#include <Windows.h>
#include <stdio.h>
#include "MinHook.h"

#ifdef _M_X64  //  If 64-bit system,

#pragma comment (lib, "libMinHook.x64.lib")

#elif _M_IX86  //  If 32-bit system,

#pragma comment (lib, "libMinHook.x86.lib")

#endif

typedef int(__stdcall* msgbox) (HWND, LPCSTR, LPCSTR, UINT);
msgbox g_pMessageBoxA = NULL;

INT __stdcall FakeMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	printf("Legit Params{lpText: %s, lpCaption: %s}", lpText, lpCaption);

	return g_pMessageBoxA(hWnd, "Tempered Output", "Tempered Caption", uType);
}

BOOL HookMsgBox() {
	DWORD dwMinhookErr = 0;

	dwMinhookErr = MH_Initialize();
	if (dwMinhookErr != MH_OK) {
		printf("[!] MH_Initialize Failed with an Error: %d\n", dwMinhookErr);
		return FALSE;
	}

	dwMinhookErr = MH_CreateHook(&MessageBoxA, &FakeMessageBoxA, &g_pMessageBoxA);
	if (dwMinhookErr != MH_OK) {
		printf("[!] MH_CreateHook Failed with an Error: %d\n", dwMinhookErr);
		return FALSE;
	}

	dwMinhookErr = MH_EnableHook(&MessageBoxA);
	if (dwMinhookErr != MH_OK) {
		printf("[!] MH_EnableHook Failed with an Error: %d\n", dwMinhookErr);
		return FALSE;
	}

	return TRUE;
}

BOOL UnHook() {
	DWORD dwMinhookErr = 0;

	dwMinhookErr = MH_DisableHook(&MessageBoxA);
	if (dwMinhookErr != MH_OK) {
		printf("[!] MH_DisableHook Failed with an Error: %d\n", dwMinhookErr);
		return FALSE;
	}

	dwMinhookErr = MH_Uninitialize();
	if (dwMinhookErr != MH_OK) {
		printf("[!] MH_DisableHook Failed with an Error: %d\n", dwMinhookErr);
		return FALSE;
	}

	return TRUE;
}

int main() {

	MessageBoxA(NULL, "Still safe from the hook?", "Legit Box", MB_YESNO); // Original MessageBoxA is called

	if (!HookMsgBox())
		return 1;

	MessageBoxA(NULL, "Still safe from the hook?", "Legit Box", MB_YESNO); // Our CustomMessageBoxA is called

	if (!UnHook())
		return 1;

	return 0;
}