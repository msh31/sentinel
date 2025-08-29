//
// Created by Marco on 23/08/2025.
//

#include <sentinel/core/utils/registry_helper.h>

// TODO: Refactor to use modern C++ style (e.g. C++ casts (reinterpret_cast, static_cast))

std::string registry_helper::getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName)
{
	HKEY rawKey;
	char buffer[512];
	DWORD dataSize = sizeof(buffer);
	DWORD type;

	if (RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &rawKey) == ERROR_SUCCESS) {
		raii_wrapper<HKEY, RegCloseKey> key(rawKey);

		if (RegQueryValueExA(key, valueName.c_str(), nullptr, &type, (LPBYTE)buffer, &dataSize) == ERROR_SUCCESS) {
			return std::string(buffer); // NOLINT(*-return-braced-init-list)
		}
	}
	return "Unknown";
}

bool registry_helper::setRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& valueData)
{
	HKEY rawKey;
	DWORD disposition;
	bool success = false;

	if (RegCreateKeyExA(hKey, subKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &rawKey, &disposition) == ERROR_SUCCESS) {
		raii_wrapper<HKEY, RegCloseKey> key(rawKey);

		if (RegSetValueExA(key, valueName.c_str(), 0, REG_SZ, (LPBYTE)valueData.c_str(), valueData.length() + 1) == ERROR_SUCCESS) {
			success = true;
		}
	}

	return success;
}