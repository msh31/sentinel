//
// Created by Marco on 22/08/2025.
//

#ifndef REGISTRY_HELPER_H
#define REGISTRY_HELPER_H

#include <sentinel/common.h>
#include <windows.h>

class registry_helper
{
	public:
		static std::string getRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName);
		static bool setRegistryValue(HKEY hKey, const std::string& subKey, const std::string& valueName, const std::string& valueData);
};

#endif //REGISTRY_HELPER_H
