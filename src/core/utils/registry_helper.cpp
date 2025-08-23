//
// Created by Marco on 23/08/2025.
//

#include <sentinel/core/utils/registry_helper.h>

std::string registry_helper::getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName)
{
	HKEY key;
	char buffer[512];
	DWORD dataSize = sizeof(buffer);
	DWORD type;

	if (RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &key) == ERROR_SUCCESS) {
		if (RegQueryValueExA(key, valueName.c_str(), nullptr, &type, (LPBYTE)buffer, &dataSize) == ERROR_SUCCESS) {
			RegCloseKey(key);
			return std::string(buffer);
		}
		RegCloseKey(key);
	}
	return "Unknown";
}