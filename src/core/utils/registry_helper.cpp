//
// Created by Marco on 23/08/2025.
//

#include <sentinel/core/utils/registry_helper.h>

// TODO: Refactor to use modern C++ style (e.g. C++ casts (reinterpret_cast, static_cast))

raii_wrapper<HKEY, RegCloseKey> registry_helper::openRegistryKey(HKEY root, const std::string& subKey)
{
	HKEY rawKey = nullptr;

	if (RegOpenKeyExA(root, subKey.c_str(), 0, KEY_READ, &rawKey) != ERROR_SUCCESS) {
		logger().error("[Registry] Failed to open registry key: " + subKey);
		return raii_wrapper<HKEY, RegCloseKey>(nullptr);  // invalid handle
	}

	return raii_wrapper<HKEY, RegCloseKey>(rawKey);
}


raii_wrapper<HKEY, RegCloseKey> registry_helper::createOrOpenRegistryKey(HKEY root, const std::string& subKey)
{
	HKEY rawKey;
	DWORD disposition;

	if (RegCreateKeyExA(root, subKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &rawKey, &disposition) != ERROR_SUCCESS) {
		return raii_wrapper<HKEY, RegCloseKey>(nullptr);
	}

	return raii_wrapper<HKEY, RegCloseKey>(rawKey);
}

std::string registry_helper::getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName)
{
	auto key = openRegistryKey(hKey, subKey);

	if (!key) {
		logger().error("[Registry] Invalid key handle when getting value: " + valueName);
		return "Unknown";
	}

	char buffer[512];
	DWORD dataSize = sizeof(buffer);
	DWORD type;

	if (RegQueryValueExA(key, valueName.c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &dataSize) == ERROR_SUCCESS) {
		return std::string(buffer);
	}

	logger().error("[Registry] Failed to read value: " + valueName);
	return "Unknown";
}

// also creates the key if it does not exist
bool registry_helper::setRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& valueData)
{
	auto key = createOrOpenRegistryKey(hKey, subKey);

	if (!key) {
		logger().error("[Registry] Failed to create/open key: " + subKey);
		return false;
	}

	return RegSetValueExA(key, valueName.c_str(), 0, REG_SZ, (LPBYTE)valueData.c_str(), static_cast<DWORD>(valueData.length() + 1)) == ERROR_SUCCESS;
}

// subkeys will not work
bool registry_helper::removeRegistryKeyAndValue(HKEY hKey, const std::string& subKey)
{
	return RegDeleteKeyA(hKey, subKey.c_str()) == ERROR_SUCCESS;
}