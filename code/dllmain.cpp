#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <psapi.h>
#define UNICODE

LPVOID dest;
BYTE OrgBytes[5] = { 0, };

BOOL MyHookFunc(HWND hwnd, HDC  hdcBlt, UINT nFlags) {
	HWND MEmu = NULL;
	char debug_buf[100];
	HMODULE hMod;
	HANDLE hProc = NULL;
	LPVOID pDestFuncAddr;
	DWORD pGapOfAddress, dwOldProtect;
	BYTE NewBytes[5] = { 0xE9,0, };
	BOOL ret;
	MEmu = FindWindow(0, TEXT("MEmu"));
	if (MEmu == hwnd) {
		return false;
	}
	else {
		VirtualProtect(dest, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		memcpy(dest, OrgBytes, 5);
		VirtualProtect(dest, 5, dwOldProtect, &dwOldProtect);
		ret = PrintWindow(hwnd, hdcBlt, nFlags);

		hMod = GetModuleHandleA("User32.dll");
		pDestFuncAddr = (LPVOID)GetProcAddress(hMod, "PrintWindow");
		dest = pDestFuncAddr;
		memcpy(&OrgBytes[0], dest, 5);
		pGapOfAddress = (DWORD)&MyHookFunc - (DWORD)pDestFuncAddr - 5;
		memcpy(&NewBytes[1], &pGapOfAddress, 4);
		VirtualProtect(pDestFuncAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		memcpy(pDestFuncAddr, NewBytes, 5);
		VirtualProtect(pDestFuncAddr, 5, dwOldProtect, &dwOldProtect);

		return ret;
	}
}
DWORD WINAPI HookCode(LPVOID lparam) {
	HMODULE hMod;
	HANDLE hProc = NULL;
	LPVOID pDestFuncAddr;
	DWORD pGapOfAddress, dwOldProtect;
	BYTE NewBytes[5] = { 0xE9,0, };

	hMod = GetModuleHandleA("User32.dll");
	pDestFuncAddr = (LPVOID)GetProcAddress(hMod, "PrintWindow");
	dest = pDestFuncAddr;
	memcpy(&OrgBytes[0], dest, 5);
	pGapOfAddress = (DWORD)&MyHookFunc - (DWORD)pDestFuncAddr - 5;
	memcpy(&NewBytes[1], &pGapOfAddress, 4);
	VirtualProtect(pDestFuncAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(pDestFuncAddr, NewBytes, 5);
	VirtualProtect(pDestFuncAddr, 5, dwOldProtect, &dwOldProtect);
	return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	HANDLE hThread;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			hThread = CreateThread(NULL, 0, HookCode, NULL, 0, NULL);
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}