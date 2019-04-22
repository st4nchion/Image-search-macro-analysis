# Analysis-and-Summary

<pre><code>
Made by Lapid, CAT-Security

If you want to use something in this documents, please state the source.

thk
</code></pre>
* * *

Which macro?
--------------------
Many people play games and use macros through android emulators.
Especially, lots of macros are operating based on **image search** and these macros cause great financial damage to game makers and companies.
So I wanted to attach it to an Android emulator to create a module that could prevent these macros.


HOW??
-------------------
Most macros with image search function are usually operated using **AHK or opencv**.
So while searching for macro that contain this function, i found dll that user created their own for make easier to use in AHK.

<center><img src = "https://user-images.githubusercontent.com/41680753/55665052-85283980-5873-11e9-9f32-5cf163f7636d.png"></center>

Now let's use IDA PRO to analyze that DLL.

**Every analysize was acheived by IDA PRO**

ANALYSIS
-------------------

![imagesearch_param](https://user-images.githubusercontent.com/41680753/55665309-084b8e80-5878-11e9-83a5-cd6a5c32cde0.PNG)

The above picture shows the function of image search and information about the factors of the function.

* **Parameter Explain**
  - str loc : directory name where the image file to find is located
  - str image : image name that want to find (exclude Extensions)
  - int x : x-coordinate in the upper left hand corner of the image
  - int y : y-coordinate in the upper left hand corner of the image
  - uint _hWnd : the handle of the window in which the image is searched
  - double acc : accuracy of image searching
  - int click_flag : flag for click or not when image is detected (0 == not click / 1 == click) 
  --> __this parameter is not important__
  
**1. How does the function bring information about the image it wants to find?**
------------------------------------------------------------------------------
**All actions are operated within the above function**

  ![getcurrentdirectoryW](https://user-images.githubusercontent.com/41680753/55665601-a2620580-587d-11e9-9456-3bdc6e109101.PNG)
  
  Gets the absolute path of the macro is exist (current process) and stores it in BUFFER with a size of 100.
  
  [detail information about GetCurrentDirectoryW] : https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcurrentdirectory
  
  ![source_image_append_bmp](https://user-images.githubusercontent.com/41680753/55667034-21613900-5892-11e9-94b0-d1a74af7c5f6.PNG)
  
  1. Save the directory name that contain image want to find in location_1
  2. Append the image name to the location_1 (directory name) and Save in location_append_image_name
  3. Append the extension "bmp" to the location_append_image_name and Save in v21
  4. Store information from v21 in v55
  
  ![abs_loction](https://user-images.githubusercontent.com/41680753/55665888-8ceeda80-5881-11e9-9ef7-8defd070b6f3.PNG)
  
  Save source image variable by combining V55 with the absolute path that was stored in the buffer.
  Then the absolute path for which we want to find images is completed.
  
  ![loadimagew](https://user-images.githubusercontent.com/41680753/55665969-eacff200-5882-11e9-8a6c-4d1e2d34758f.PNG)

  Loads the stand-alone image from the file specified by lpszName (image that we want to find) and Save in bitmap variable.
  
  [detail information about LoadImageW] : https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-loadimagew
  
  ![source_image_dc](https://user-images.githubusercontent.com/41680753/55666234-8b73e100-5886-11e9-9ff4-600ae603743a.PNG)
  
  Let's look inside the function sub_18060DED0.
  
  ![compatibledc](https://user-images.githubusercontent.com/41680753/55666309-07baf400-5888-11e9-9f37-abc290a5babf.PNG)
  
  1. In line 2, Creates a memory DC compatible with the application's current screen and Save the handle to a memory DC in v3.
  2. Then, After V3 and V2 are passed as arguments, return a handle to the object being replaced.
  
  [detail information about createcompatibledc] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createcompatibledc
  
  [detail information about selectobject] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-selectobject

  ![getdibits](https://user-images.githubusercontent.com/41680753/55666467-4a7dcb80-588a-11e9-87b9-26d28a59d5e7.PNG)
  
  After get v3 and v2 as arguments (hdc and hbitmap) and Operate, Save in a pointer to a bmi structure that specifies the desired format for the DIB data.
  
  [detail information about GetDIBits] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-getdibits
  
  ![create_opencv_class](https://user-images.githubusercontent.com/41680753/55666556-af85f100-588b-11e9-9778-5894436bf889.png)
  
  Put v5 and BMI.BMIHEADER.BIWIDTH into the arguments of the function.
  And in Concurrency::details::SchedulerBase::Create function 
  
  ![create_opencv_class_2](https://user-images.githubusercontent.com/41680753/55666574-f83daa00-588b-11e9-940e-b0d6c3d58a78.png)
  
  Using the v5 and bmi.bmiHeader.biwidth received by arguments, calculate the operation in the cvCreateData function and return the value to v26.
  
  [detail information about cvCreateData] : https://docs.opencv.org/2.4/modules/core/doc/old_basic_structures.html?highlight=cvcreatedata#void%20cvCreateData(CvArr*%20arr)
  
  ![img_last](https://user-images.githubusercontent.com/41680753/55668193-44471980-58a1-11e9-9db8-afaf9d299454.PNG)
  
  **So far, we've analyzed the parts that bring information about the images we want to find.**
  
**2. How does the function get information about the window to which the image is to be found?**
------------------------------------------------------------------------------
**All actions are operated within the above function**

![memu_1](https://user-images.githubusercontent.com/41680753/55667537-70aa6800-5898-11e9-92f6-c45834877fbc.PNG)

Using GetWindowDc function and argument hWnda(handle of android emulator), retrieve the android emulator's device context and store a handle to a device context for the specialized window in the HDC variable.

![memu_2](https://user-images.githubusercontent.com/41680753/55667538-71db9500-5898-11e9-8192-d180ba002f2f.PNG)

And then hdc, hWnda and variables needed for operation are passed on to __printwindow function. 

![memu_3](https://user-images.githubusercontent.com/41680753/55667541-73a55880-5898-11e9-9b64-ad89908e9a83.PNG)

In __printwindow function, we can see a configuration similar to when we bring information about the image we want to find.
Among them, we can see in the second row that we haven't seen before.
This function creates a bitmap compatible with the device that is associated with the specified device context.
* **Arguments Explain**
  - v9 : DC of andriod emulator
  - v6 : bitmap width, in pixels
  - v8 : bitmap height, in pixels
After opration, this function return a handle to the compatible bitmap

Other functions operate the same as before.

[detail information about createcompatiblebitmap] : https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createcompatiblebitmap

![memu_4](https://user-images.githubusercontent.com/41680753/55667809-3a6ee780-589c-11e9-99c6-4a7a6f5fbfc9.PNG)

This function is important element of macro based on image searching.
It copies a visual window into the specified device context, typically a printer DC.

[detail information about printwindow] : https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-printwindow

![memu_5](https://user-images.githubusercontent.com/41680753/55668194-45784680-58a1-11e9-8ddb-e7fa8575f943.PNG)

Just as in the case above,This part also receives the height and width of the image as a arguments and calculate the operation in the cvCreateData function and return the value.

**Now the MEMU variable contains information about the screen of the window to compare the images we want to find**

**3. Let's compare android emulator's image and image we want to find**
------------------------------------------------------------------------------
**All actions are operated within the above function**

Now that we have stored all the information about image we want to find and the screen of the window we want to compare to the image we want to find, we have to use it to calculate the accuracy of each other.

![acc_1](https://user-images.githubusercontent.com/41680753/55668621-5f1c8c80-58a7-11e9-844e-8f6ef538f0dc.PNG)

This is a function that compares the image to be found and the image to be compared.

Let's go inside this function.

![acc_3](https://user-images.githubusercontent.com/41680753/55668780-42815400-58a9-11e9-82f2-4bca891db896.PNG)

Using this function, compares a template against overlapped image regions.

And then

[detail information about cv.MatchTemplate] : https://docs.opencv.org/2.4/modules/imgproc/doc/object_detection.html?highlight=matchtemplate#cv.MatchTemplate

![acc_2](https://user-images.githubusercontent.com/41680753/55668623-5f1c8c80-58a7-11e9-8118-a2fe695a0f74.PNG)

Let's look inside the function just below.

![acc_4](https://user-images.githubusercontent.com/41680753/55668808-8e33fd80-58a9-11e9-8c15-c750d7b007a1.PNG)


Using the function cv.minmaxLoc, calculate the maximum and minimum similarity of the two images and store maximum similarity in variable v60.

![acc_last](https://user-images.githubusercontent.com/41680753/55668969-62fedd80-58ac-11e9-8037-701ce93fd889.PNG)

At last, compare maximum accuracy and accuracy we've entered.
If maximum accuracy is lower than accuracy we've entered, return 0 or 1.

[detail information about cv.minmaxLoc] : https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#void%20minMaxLoc(InputArray%20src,%20double*%20minVal,%20double*%20maxVal,%20Point*%20minLoc,%20Point*%20maxLoc,%20InputArray%20mask)

![acc_5](https://user-images.githubusercontent.com/41680753/55668930-9d1baf80-58ab-11e9-99e2-e8f5af1180c0.PNG)

However, because it subtracts 1 from the corresponding return value at the end of ImageSearchAndClick function, it returns 0 if the image is ultimately found and -1 if not found.


# PROTECTION TECHNIQUE

We used the method of disabling that macro through dll injection.
To inject dll into all processes, we made 32-bit, 64-bit injectors and dlls.

* injector code
<pre><code>#include <windows.h>
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
}</code></pre>

The source code above is the complete code of the injector.
A major part of this is
<pre><code>BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath) {
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
}</pre></code>

We used CreateRemoteThread to do dll injection.
The CreateRemoteThread function creates a thread in the virtual address space of an arbitrary process. Let’s take a look at the parameters we must pass to the functions, which we can see on the picture below.

![image](https://user-images.githubusercontent.com/41680753/56423668-2d181b00-62e8-11e9-8228-44bd6e1a481c.png)

It’s immediately clear that we must pass a value in the first six parameters, but the function writes some value into the last parameter. Let’s describe the parameters a little bit more.

- hProcess: handle to the process where we’ll create a new thread
- lpThreadAttributes: a pointer to the SECURITY_ATTRIBUTES structure, which specifies the security attributes of the new thread: if NULL, the thread will have default security attributes and the handle cannot be inherited by the child process
- dwStackSize: initial size of the stack
- lpStartAddress: a pointer to the LPTHREAD_START_ROUTINE, which is a function that will be executed by the new thread. It’s needless to say that the function must exists in the remote process.
- lpParameter: a pointer to a variable to be passed to the thread function
- dwCreationFlags : a value that controls the creation of the thread
- ThreadId: a pointer to a variable that receives the thread ID

If the function succeeds, the returned value is a handle to the new thread. Otherwise, the function returns NULL.

[more information about CreateRemoteThread] : https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createremotethread

We’ve just seen that the CreateRemoteThread function can be used to start a new thread in the address space of some process.
Now it’s time to present the whole process we’ll be using to inject a DLL into the process’ address space. To get a clear indication of what we’re going to do, take a look at the picture below, where the process we’ll be injecting a DLL into is marked with purple color and has a name victim.exe. But, there are two other pieces of the puzzle we need to clarify. First, we need to establish that if we want to inject a DLL into some process, we must first have the DLL we would like to inject. The DLL is presented with the green color and has a name inject.dll. But we must also have a program that will do the injection of the DLL into the victim’s address space. That program is presented in blue and has a name program.exe

![image](https://user-images.githubusercontent.com/41680753/56491218-c3d01c00-6522-11e9-9247-c4c1e121ad7b.png)

The program.exe must call the presented functions sequentially in order to be able to inject a DLL into the victim’s address space. First, it must call OpenProcess to get a handle to the victim’s process. Afterwards it must call GetProcAddress function to get the address of the LoadLibrary function inside the kernel32.dll library; here we can run any function we like, but it must be present in a DLL, which is already loaded in the process’s address space. We know that every program uses kernel32.dll library, so the best way to inject a DLL into the process’s address space is looking for the LoadLibraryA function and calling that. In order for our DLL to be loaded, we must pass a DLL path to the LoadLibraryA function, but the name needs to be stored somewhere inside the processes address space. Obviously, it’s highly unlikely for the path to our DLL to already be present somewhere in the process’s address space, which is why we need the next two functions: VirtualAllocEx and WriteProcessMemory. The first function allocates a new memory range inside the process’s address space. The size of that memory region needs to be only as large to fit the name of the DLL inside it; usually the size is rounded up to occupy at least one page. The WriteProcessMemory is the function that actually writes the path of our DLL to the victim’s address space. At last, the CreateRemoteThread is called that calls the LoadLibrary function inside the victim’s address space to inject a DLL into it.

* dynamic link library code
<pre><code>#include <Windows.h>
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
	MEmu = FindWindow(0, TEXT("MEmu")); // get memu's handle
	if (MEmu == hwnd) {
		return false;	
	}
	else {
		VirtualProtect(dest, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect); // Changes the protection on a region of committed pages in the virtual address space of the calling proces.
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
}</pre></code>

If another process takes the handle of the app player and uses the handle of the app player for the PrintWindow function, it will not recognize the image through PrintWindow by giving the screen of the app player but the screen of another thread created by the CreateThread function.

Refer to the source code for more information.

* process monitor(auto injector) code

<code><pre>while (1){
	NEW_PID.clear();
	pe32.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(hProcessSnap, &pe32);
	do{
		IsOverlap = 0;
		if (((find(NEW_PID.begin(), NEW_PID.end(), pe32.th32ProcessID)) == NEW_PID.end())){
			NEW_PID.insert(NEW_PID.end(), pe32.th32ProcessID);
			IsOverlap = 1;
		}
		if (((find(OLD_PID.begin(), OLD_PID.end(), pe32.th32ProcessID)) == OLD_PID.end()) && IsOverlap){
			TCHAR szPID[500] = {0};
			SHELLEXECUTEINFO ShRun = {0};
			_itot_s(pe32.th32ProcessID, szPID, 10);
			_tcscat(szPID, TEXT(" "));
			ShRun.cbSize = sizeof(SHELLEXECUTEINFO);
			ShRun.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShRun.hwnd = NULL;
			ShRun.lpVerb = NULL;
			ShRun.nShow = SW_HIDE;
			ShRun.hInstApp = NULL;
			if (IsWow64(pe32.th32ProcessID)){
				_tcscat(szPID, DLL32_PATH);
				ShRun.lpFile = Injector32_PATH;
			}
			else{
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
</pre></code>
