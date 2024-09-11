#include <Windows.h>
#include <stdio.h>

VOID MsgBoxPayload() {
    MessageBoxA(NULL, "Hacking By Mabenchi", "Wow !", MB_OK | MB_ICONINFORMATION);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {

    switch (dwReason) {
    case DLL_PROCESS_ATTACH: {
        MsgBoxPayload();
        break;
    };
    case DLL_THREAD_ATTACH: {
        MsgBoxPayload();
        break;
    }
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}