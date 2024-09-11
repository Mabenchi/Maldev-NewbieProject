#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

BOOL UuidDeobfuscation(IN CHAR* UuidArray[], IN SIZE_T NmbrOfElements, OUT PBYTE* ppDAddress, OUT SIZE_T* pDSize);
