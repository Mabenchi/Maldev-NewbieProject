#pragma once
#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>


BOOL InjectDllToRemoteProcess(IN HANDLE hProcess, IN LPWSTR DllName);
BOOL GetRemoteProcessHandle(IN LPWSTR szProcessName, OUT DWORD* dwProcessId, OUT HANDLE* hProcess);
wchar_t* toLowerCase(const wchar_t* str);
