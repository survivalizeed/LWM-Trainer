#pragma once

#include <iostream>
#include <string_view>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <map>
#include <concepts>


class Trainer {
	HANDLE hProcess;
	std::string_view procName;
	DWORD procId;
	uintptr_t modBase;
	std::map<std::string_view, uintptr_t> entrys;
	
public:
	Trainer(std::string_view procName)
		: procName(procName), procId(GetProcessID()), modBase(GetModuleBaseAddress()) 
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
	}

	void AddEntry(std::string_view name, uintptr_t dynamicBaseOffset, const std::vector<unsigned int>& offsets);

	template<typename Ret>
	Ret& Read(std::string_view name) {
		Ret value{};
		ReadProcessMemory(hProcess, (BYTE*)entrys[name], &value, sizeof(value), nullptr);
		return value;
	}

	template<typename Type>
	requires std::integral<Type>
	void Write(std::string_view name, Type value) {
		WriteProcessMemory(hProcess, (BYTE*)entrys[name], &value, sizeof(value), nullptr);
	}

	template<typename Ret>
	Ret ReadDynAddress(uintptr_t address) {
		Ret buf{};
		ReadProcessMemory(hProcess, (BYTE*)address, &buf, sizeof(buf), nullptr);
		return buf;
	}

	void WriteDynAddress(uintptr_t address, const std::vector<BYTE>& bytes) {
		for (auto& iter : bytes) {
			WriteProcessMemory(hProcess, (BYTE*)address, &iter, sizeof(BYTE), nullptr);
			address += 0x1;
		}
	}

private:
	DWORD GetProcessID();

	uintptr_t GetModuleBaseAddress();

};
