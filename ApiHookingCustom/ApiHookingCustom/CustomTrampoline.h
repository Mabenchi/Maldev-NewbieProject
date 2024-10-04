#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#ifdef _WIN64 // 64-bit system

#define PAYLOAD_SIZE 13

#elif _WIN32 // 32-bit system

#define PAYLOAD_SIZE 7

#endif 


typedef struct _SHOOK {
	PVOID pTargetFunction;
	PVOID pNewFunction;
	PVOID pOriginalInstructions[PAYLOAD_SIZE];
	DWORD OldProtect;

}SHOOK, * PSHOOK;

BOOL	InitializeHook(IN PVOID pTargetFunction, IN PVOID pNewFunction, OUT PSHOOK psHook);

BOOL	InstallHook(IN PSHOOK psHook);

BOOL	UnInstallHook(IN PSHOOK psHook);