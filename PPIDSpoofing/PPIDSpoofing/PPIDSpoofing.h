#pragma once
#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

BOOL GetRemoteProcessHandle(LPWSTR szProcessName, DWORD* dwProcessId, HANDLE* hProcess);
BOOL CreatePPidSpoofedProcess(IN HANDLE hTargetProcess, IN LPCWSTR lpProgramPath, OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread);