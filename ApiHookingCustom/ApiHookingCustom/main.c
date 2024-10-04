#include "CustomTrampoline.h"

typedef int(__stdcall* msgbox) (HWND, LPCSTR, LPCSTR, UINT);

INT __stdcall FakeMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	printf("Legit Params{lpText: %s, lpCaption: %s}\n", lpText, lpCaption);

	return MessageBoxW(hWnd, L"Tempered Output", L"Tempered Caption", uType);
}



int main() {

	SHOOK sHook;

	MessageBoxA(NULL, "Still safe from the hook?", "Legit Box", MB_YESNO); // Original MessageBoxA is called

	if (!InitializeHook(MessageBoxA, FakeMessageBoxA, &sHook))
		return 1;

	if (!InstallHook(&sHook))
		return 1;

	MessageBoxA(NULL, "Still safe from the hook?", "Legit Box", MB_YESNO); // Our CustomMessageBoxA is called

	if (!UnInstallHook(&sHook))
		return 1;

	MessageBoxA(NULL, "Still safe from the hook?", "Legit Box", MB_YESNO); // Original MessageBoxA is called
	return 0;
}