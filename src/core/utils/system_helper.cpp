//
// Created by Marco on 25/08/2025.
//

#include <sentinel/core/utils/system_helper.h>
#include <sentinel/core/utils/registry_helper.h>
#include <sentinel/core/logger.h>
#include <sentinel/common.h>

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

	switch (sysInfo.wProcessorArchitecture) { //this is a WORD value which is an 16-bit unsigned integer meaniing it can be used in a switch statement, apparently thats a thiing
		case PROCESSOR_ARCHITECTURE_AMD64:
			return std::string("x64");
		case PROCESSOR_ARCHITECTURE_ARM:
			return std::string("ARM");
		case PROCESSOR_ARCHITECTURE_ARM64:
			return std::string("ARM64");
		case PROCESSOR_ARCHITECTURE_INTEL:
			return std::string("x86");
	}

	return "Unknown";
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
