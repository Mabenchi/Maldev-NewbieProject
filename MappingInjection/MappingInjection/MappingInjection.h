#pragma once
#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#pragma comment (lib, "OneCore.lib")

BOOL LocalMapInjector(PBYTE pPayload, SIZE_T sPayloadSize, PVOID* ppAddress);
BOOL RemoteMapInjector(HANDLE hProcess, PBYTE pPayload, SIZE_T sPayloadSize, PVOID* ppAddress);