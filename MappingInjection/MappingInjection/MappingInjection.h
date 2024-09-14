#pragma once
#include <windows.h>
#include <stdio.h>

BOOL LocalMapInjector(PBYTE pPayload, SIZE_T sPayloadSize, PVOID* ppAddress);
