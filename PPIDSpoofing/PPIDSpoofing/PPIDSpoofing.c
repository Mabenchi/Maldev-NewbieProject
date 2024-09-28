#include "PPIDSpoofing.h"

int wmain(int ac, wchar_t** av)
{
	HANDLE hTargetProcess		= NULL;
	DWORD  dwProcessId			= 0;
	HANDLE hSpoofedProcess		= NULL;
	HANDLE hSpoofedMainThread	= NULL;

	if (ac != 3) {
		printf("Usage: .\\PPIDSpoofing.exe <Target Process Name> <Program Path>\n");
		return 1;
	}

	if (!GetRemoteProcessHandle(av[1], &dwProcessId, &hTargetProcess)) {
		printf("[!] GetRemoteProcessHandle Failed to retrieve Process Handle\n");
		return 1;
	}

	if (!CreatePPidSpoofedProcess(hTargetProcess, av[2], &dwProcessId, &hSpoofedProcess, &hSpoofedMainThread)) {
		printf("[!] CreatePPidSpoofedProcess Failed\n");
		return 1;
	}

	return 0;

}