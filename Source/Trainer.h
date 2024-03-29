#pragma once

#include <iostream>
#include <string_view>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <map>
#include <concepts>
#include <utility>
#include <thread>
#include <functional>


class Trainer {
	bool procValid = false;
	bool killThread = false;
	HANDLE hProcess;
	std::string_view procName;
	std::string procPath;
	DWORD procId;
	uintptr_t modBase;
	std::map<std::string_view, uintptr_t> entrys;
	std::map<std::string_view, std::pair<uintptr_t, std::vector<BYTE>>> stores;

public:

	Trainer(std::string_view procName)
		: procName(procName), procId(GetProcessID()), modBase(GetModuleBaseAddress())
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		DWORD size = MAX_PATH;
		char* filename = new char[MAX_PATH];
		QueryFullProcessImageNameA(GetProcHandle(), 0, filename, &size);
		procPath = filename;
		delete[] filename;
	}

	void StartCheckThread();

	void AddEntry(std::string_view name, uintptr_t dynamicBaseOffset, const std::vector<unsigned int>& offsets);

	void AddStaticEntry(std::string_view name, uintptr_t staticAddress);

	bool IdleWait(std::string_view searchmessage, std::string_view foundmessage, std::function<void()> callback);

	template<typename Ret>
	Ret Read(std::string_view name) {
		Ret value{};
		ReadProcessMemory(hProcess, (BYTE*)entrys[name], &value, sizeof(value), nullptr);
		return value;
	}

	template<typename Type>
		requires std::integral<Type> || std::floating_point<Type>
	void Write(std::string_view name, Type value) {
		WriteProcessMemory(hProcess, (BYTE*)entrys[name], &value, sizeof(value), nullptr);
	}

	template<typename Ret>
	Ret ReadAddress(uintptr_t startaddress) {
		Ret buf{};
		ReadProcessMemory(hProcess, (BYTE*)startaddress, &buf, sizeof(buf), nullptr);
		return buf;
	}

	void WriteAddress(uintptr_t startaddress, const std::vector<BYTE>& bytes);

	void Patch(std::string_view name, uintptr_t startaddress, const std::vector<BYTE>& instructions);

	void Restore(std::string_view name);

	// Before using this, you must call StartCheckThread()
	void DiskPatch(uintptr_t rawAddress, std::vector<BYTE> instructions);

	void Freeze();

	void Unfreeze();

	bool Reload();

	bool StillValid();

	DWORD GetProcId();

	HANDLE GetProcHandle();

	std::string GetProcPath();

	~Trainer();

private:
	DWORD GetProcessID();

	uintptr_t GetModuleBaseAddress();

};
