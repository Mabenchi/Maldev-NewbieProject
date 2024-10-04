#include "CustomTrampoline.h"

BOOL	InitializeHook(IN PVOID pTargetFunction, IN PVOID pNewFunction, OUT PSHOOK psHook) {

	// Initialize the struct
	psHook->pTargetFunction = pTargetFunction;
	psHook->pNewFunction = pNewFunction;

	// Keep a backup of the origibal instructions to restore it after unhooking
	memcpy(psHook->pOriginalInstructions, psHook->pTargetFunction, PAYLOAD_SIZE);

	// Change the target memory to read, write, execute since we want to write and execute our trampoline here
	if (!VirtualProtect(psHook->pTargetFunction, PAYLOAD_SIZE, PAGE_EXECUTE_READWRITE, &(psHook->OldProtect))) {
		printf("[!] VirtualProtect pTargetFunction Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL	InstallHook(IN PSHOOK psHook) {
#ifdef _WIN64

	// Opcode of the trampoline payload
	unsigned char trampoline[] = {
	0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs r10, 0x0
	0x41, 0xFF, 0xE2                                            // jmp r10
	};

#elif _WIN32

	// Opcode of the trampoline payload
	unsigned char trampoline[] = {
	0xb8, 0x00, 0x00, 0x00, 0x00, // mov eax, 0x0
	0xFF, 0xE0                    // jmp eax
	};

#endif // _WIN64

	// Copy address of the new function to the payload
	memcpy(&trampoline[2], &(psHook->pNewFunction), sizeof(psHook->pNewFunction));

	// Install the hook on the target function
	memcpy(psHook->pTargetFunction, trampoline, sizeof(trampoline));
	printf("InstallHook %p\n", psHook->pTargetFunction);
	return TRUE;
}

BOOL	UnInstallHook(IN PSHOOK psHook) {

	// Restoring the instruction of the target function
	memcpy(psHook->pTargetFunction, psHook->pOriginalInstructions, PAYLOAD_SIZE);

	// Restore the previous permession
	if (!VirtualProtect(psHook->pTargetFunction, PAYLOAD_SIZE, psHook->OldProtect, &(psHook->OldProtect))) {
		printf("[!] VirtualProtect UnInstallHook Failed with an Error: %ld\n", GetLastError());
		return FALSE;
	}

	// Cleanup the struct
	psHook->OldProtect = 0;
	psHook->pNewFunction = NULL;
	psHook->pTargetFunction = NULL;
	memset(psHook->pOriginalInstructions, 0, PAYLOAD_SIZE);


	return TRUE;
}