#pragma once
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

BOOL GetRemoteThread(IN DWORD dwProcessId, OUT DWORD* dwThreadId, OUT HANDLE* hThread);
BOOL HijackThread(HANDLE hThread, PVOID pAddress);