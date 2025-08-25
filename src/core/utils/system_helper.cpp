//
// Created by Marco on 25/08/2025.
//

#include <sentinel/core/utils/system_helper.h>
#include "sentinel/core/utils/registry_helper.h"

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