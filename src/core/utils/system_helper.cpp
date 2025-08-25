//
// Created by Marco on 25/08/2025.
//

#include <sentinel/core/utils/system_helper.h>
#include "sentinel/core/utils/registry_helper.h"

#include <filesystem>

std::string system_helper::getComputerName() {
	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName);

	if (GetComputerNameA(computerName, &size)) {
		return std::string(computerName); // NOLINT(*-return-braced-init-list)
	}

	return "Unknown";
}

std::string system_helper::getSystemProductName() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "SystemProductName");
}

std::string system_helper::getBIOSManufacturer() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "SystemManufacturer");
}
std::string system_helper::getBIOSVersion() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "BIOSVersion");
}

std::string system_helper::getVideoBiosVersion() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "VideoBiosVersion");
}

std::string system_helper::getArchitectureString(const SYSTEM_INFO& sysInfo){
	// for reference, it's labeled as AMD64 simply because AMD got there first, Intel CPU's these days use it too, before that it was x86
	// TODO: Improve return value by checking for more architecture types, like X86 & Arm(64)
	return std::string(sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? "x64" : "x86");
}

std::vector<std::string> system_helper::filterFilesByType(const std::vector<std::string>& files, const std::vector<std::string>& extensions)
{
	std::vector<std::string> filteredFiles;

	for (const auto& file : files) {
		std::filesystem::path filePath = file;
		std::string extension = filePath.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		for (const auto& targetExt : extensions) {
			std::string lowerTargetExt = targetExt;
			std::transform(lowerTargetExt.begin(), lowerTargetExt.end(), lowerTargetExt.begin(), ::tolower);

			if (extension == lowerTargetExt) {
				filteredFiles.push_back(file);
				break;
			}
		}
	}

	return filteredFiles;
}