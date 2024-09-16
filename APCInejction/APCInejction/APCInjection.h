#pragma once
#include <windows.h>
#include <stdio.h>

BOOL RunViaApcInjection(HANDLE hThread, PBYTE pPayload, SIZE_T sPayloadSize);