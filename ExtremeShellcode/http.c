#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

// Why is it so long to do a get request into a buffer...
size_t httpToBuffer(const char* url, char** buffer) {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    BOOL result;
    size_t bufferSize = 0;
    char* responseBuffer = NULL;
    DWORD bufferLength = 0;

    // WinInet init
    hInternet = InternetOpen(L"HTTP Client", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("Erreur lors de l'initialisation de WinINet.\n");
        return 0;
    }

    // Url connection
    hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("Erreur lors de la connexion à l'URL: %s\n", url);
        InternetCloseHandle(hInternet);
        return 0;
    }

    // Reading response by 1024 chunks
    while (1) {
        // Increase size if needed
        bufferLength += 1024;
        responseBuffer = realloc(responseBuffer, bufferLength);
        if (responseBuffer == NULL) {
            printf("Memory allocation error.\n");
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return 0;
        }

        // Read data from connection
        result = InternetReadFile(hConnect, responseBuffer + bufferSize, 1024, &bytesRead);
        if (!result || bytesRead == 0) {
            break; // We are done ! I don't want to be your wife anymore !
        }

        bufferSize += bytesRead;
    }

    // clean code
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    *buffer = responseBuffer;

    return bufferSize;
}