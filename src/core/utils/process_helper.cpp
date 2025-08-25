//
// Created by Marco on 25/08/2025.
//

#include <sentinel/core/utils/process_helper.h>
#include <sentinel/core/utils/registry_helper.h>

#include <tlhelp32.h>

std::vector<std::string> process_helper::getRunningProcesses()
{
	std::vector<std::string> processNames;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return processNames;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pe32)) {
		do {
			processNames.emplace_back(std::string(pe32.szExeFile));
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return processNames;
}