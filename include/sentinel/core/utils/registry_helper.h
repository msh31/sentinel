//
// Created by Marco on 22/08/2025.
//

#ifndef REGISTRY_HELPER_H
#define REGISTRY_HELPER_H

#include <sentinel/common.h>
#include <sentinel/core/logger.h>
#include <sentinel/core/utils/raii_wrapper.h>

class registry_helper
{
	public:
		static std::string getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName);
		static bool setRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& valueData);
		static bool removeRegistryKeyAndValue(HKEY hKey, const std::string& subKey);

	private:
		logger log;

		static raii_wrapper<HKEY, RegCloseKey> openRegistryKey(HKEY root, const std::string& subKey);
		static raii_wrapper<HKEY, RegCloseKey> createOrOpenRegistryKey(HKEY root, const std::string& subKey)
};

#endif //REGISTRY_HELPER_H
