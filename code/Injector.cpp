#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <shellapi.h>
#include <tlhelp32.h>
#define UNICODE
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath);
int _tmain(int argc, TCHAR**argv){
	if (argc != 3) {
		_tprintf(TEXT("USAGE : Injector PID DLL_PATH\n"));
	}
	else {
		InjectDll(_tstoi(argv[1]), argv[2]);
	}
}
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath) {
	HANDLE hProcess = NULL, hThread = NULL;
	HMODULE hModule = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadproc;
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID))) {
		return FALSE;
	}
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);
	hModule = GetModuleHandle(TEXT("kernel32.dll"));
	pThreadproc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryW");
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadproc, pRemoteBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}