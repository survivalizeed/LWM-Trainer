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
	std::map<std::string_view, std::vector<BYTE>> stores;

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
	Ret ReadDynAddress(uintptr_t startaddress) {
		Ret buf{};
		ReadProcessMemory(hProcess, (BYTE*)startaddress, &buf, sizeof(buf), nullptr);
		return buf;
	}

	void WriteDynAddress(uintptr_t startaddress, const std::vector<BYTE>& bytes) {
		for (auto& iter : bytes) {
			WriteProcessMemory(hProcess, (BYTE*)startaddress, &iter, sizeof(BYTE), nullptr);
			startaddress++;
		}
	}

	void Patch(std::string_view name, uintptr_t startaddress, const std::vector<BYTE>& instructions) {
		std::vector<BYTE> tmp(instructions.size());
		for (int i = 0; i < instructions.size(); ++i) {
			ReadProcessMemory(hProcess, (BYTE*)startaddress, &tmp[i], sizeof(BYTE), nullptr);
			startaddress++;
		}
		stores[name] = tmp;
		WriteDynAddress(startaddress - (instructions.size()), instructions);
	}

	void Restore(std::string name, uintptr_t startaddress) {
		WriteDynAddress(startaddress, stores[name]);
	}

private:
	DWORD GetProcessID();

	uintptr_t GetModuleBaseAddress();

};
