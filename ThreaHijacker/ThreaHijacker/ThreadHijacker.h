#pragma once

#include <windows.h>
#include <stdio.h>

int LocalThreadHijacking(HANDLE hThread, LPBYTE pPayload, SIZE_T szPayload);