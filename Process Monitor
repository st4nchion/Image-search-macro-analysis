#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <process.h> 
#include <algorithm>
#include <vector>

#pragma warning (disable:4996)

#define ICON_ID 0x1234
#define WM_TRAY 0x2345
#define MENU_QUIT_MESSAGE 0x103
#define ERROR_ALREADY_EXISTS 0xB7

using namespace std;

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("CAT-Security");
//Constraint : Paths -> Absolute Path
LPCTSTR Injector32_PATH = TEXT("C:\\Users\\gud425\\Desktop\\project\\injector_32.exe");
LPCTSTR Injector64_PATH = TEXT("C:\\Users\\gud425\\Desktop\\project\\injector_64.exe");
LPCTSTR DLL32_PATH = TEXT("C:\\Users\\gud425\\Desktop\\project\\dll_32.dll");
LPCTSTR DLL64_PATH = TEXT("C:\\Users\\gud425\\Desktop\\project\\dll_64.dll");
LRESULT CALLBACK WndProc(HWND hwnd, UINT, WPARAM wParam, LPARAM lParam);
BOOL IsFirst = TRUE;
HANDLE hMutex;

BOOL IsWow64(int num) {
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process) {
		fnIsWow64Process(OpenProcess(PROCESS_ALL_ACCESS, FALSE, num), &bIsWow64);
	}
	return bIsWow64;
}
DWORD32 WINAPI GetProcessID(LPVOID lpParam) {
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	vector<int>NEW_PID;
	vector<int>OLD_PID;
	int IsOverlap = 0;
	while (1) {
		NEW_PID.clear();
		pe32.dwSize = sizeof(PROCESSENTRY32);
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		Process32First(hProcessSnap, &pe32);
		do {
			IsOverlap = 0;
			if (((find(NEW_PID.begin(), NEW_PID.end(), pe32.th32ProcessID)) == NEW_PID.end())) {
				NEW_PID.insert(NEW_PID.end(), pe32.th32ProcessID);
				IsOverlap = 1;
			}
			if (((find(OLD_PID.begin(), OLD_PID.end(), pe32.th32ProcessID)) == OLD_PID.end()) && IsOverlap) {
				TCHAR szPID[500] = { 0 };
				SHELLEXECUTEINFO ShRun = { 0 };
				_itot_s(pe32.th32ProcessID, szPID, 10);
				ShRun.cbSize = sizeof(SHELLEXECUTEINFO);
				ShRun.fMask = SEE_MASK_NOCLOSEPROCESS;
				ShRun.hwnd = NULL;
				ShRun.lpVerb = NULL;
				ShRun.nShow = SW_HIDE;
				ShRun.hInstApp = NULL;
				_tcscat(szPID, TEXT(" "));
				if (IsWow64(pe32.th32ProcessID)) {
					_tcscat(szPID, DLL32_PATH);
					ShRun.lpFile = Injector32_PATH;
				}
				else {
					_tcscat(szPID, DLL64_PATH);
					ShRun.lpFile = Injector64_PATH;
				}
				ShRun.lpParameters = szPID;
				ShellExecuteEx(&ShRun);
				Sleep(10);
				if (IsFirst)
					Sleep(200);
			}
		} while (Process32Next(hProcessSnap, &pe32));
		if (IsFirst)
			IsFirst = FALSE;
		OLD_PID.clear();
		OLD_PID = NEW_PID;
		Sleep(100);
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	HWND hWnd;
	DWORD dwThreadId = 1;
	HANDLE hThreadc = (HANDLE)_beginthreadex(NULL, 0, GetProcessID, NULL, 0, (DWORD32 *)&dwThreadId);
	hMutex = CreateMutex(NULL, FALSE, TEXT("mutex"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, TEXT("Already Running"), TEXT("WARNING"), MB_OK);
		return 0;
	}

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = NULL;
	WndClass.hCursor = NULL;
	WndClass.hIcon = NULL;
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CAPTION, 0, 0, 500, 500, NULL, (HMENU)NULL, hInstance, NULL);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	int wmEvent, wmId;
	HMENU hMenu, hPopupMenu, hMenubar;
	hMenu = CreateMenu();
	hMenubar = CreateMenu();
	AppendMenu(hMenu, MF_STRING, MENU_QUIT_MESSAGE, TEXT("Quit"));
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, TEXT("Menu"));
	switch (iMessage) {
	case WM_CREATE:
		NOTIFYICONDATA nid;
		ZeroMemory(&nid, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.hWnd = hWnd;
		nid.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
		nid.uCallbackMessage = WM_TRAY;
		nid.uID = ICON_ID;
		nid.hIcon = (HICON)LoadImage(NULL, TEXT(".\\CAT-Security.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		lstrcpy(nid.szTip, TEXT("CAT-Security"));
		Shell_NotifyIcon(NIM_ADD, &nid);

		return 0;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId) {
		case MENU_QUIT_MESSAGE:
			exit(0);
		}
		break;
		return 0;
	case WM_TRAY:
		if (wParam == ICON_ID) {
			switch (lParam) {
			case WM_RBUTTONUP:
				hPopupMenu = GetSubMenu(hMenubar, 0);
				POINT pt;
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
				SetForegroundWindow(hWnd);
				PostMessage(hWnd, WM_NULL, 0, 0);
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
