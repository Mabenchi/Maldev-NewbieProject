#include "DLLInjection.h"

int wmain(int argc, wchar_t* argv[])
{
	DWORD processId;
	HANDLE hProcess;

	if (argc != 3)
	{
		printf("Usage: .\\DLLInjection.exe [Program Name] [DLL]");
		return 1;
	}

	if (GetRemoteProcessHandle(argv[1], &processId, &hProcess))
		printf("[i] Found processId: %d\n", processId);
	else
	{
		printf("[i] UnFound\n");
		return 1;
	}
	InjectDllToRemoteProcess(hProcess, argv[2]);
	return 0;
}