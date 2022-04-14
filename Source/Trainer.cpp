#include "Trainer.h"


void Trainer::AddEntry(std::string_view name, uintptr_t dynamicBaseOffset, const std::vector<unsigned int>& offsets)
{
	uintptr_t addr = modBase + dynamicBaseOffset;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProcess, (BYTE*)addr, &addr, sizeof(addr), nullptr);
		addr += offsets[i];
	}
	entrys[name] = addr;
}


DWORD Trainer::GetProcessID()
{
	wchar_t wBuf[100];
	mbstowcs(wBuf, procName.data(), 100);
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry{};
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				wchar_t procEn[100];
				mbstowcs(procEn, procEntry.szExeFile, 100);
				if (!_wcsicmp(procEn, wBuf))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t Trainer::GetModuleBaseAddress()
{
	wchar_t wBuf[100];
	mbstowcs(wBuf, procName.data(), 100);
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry{};
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				wchar_t modEn[100];
				mbstowcs(modEn, modEntry.szModule, 100);
				if (!_wcsicmp(modEn, wBuf))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}
