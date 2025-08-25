//
// Created by Marco on 25/08/2025.
//

#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <sentinel/common.h>

#include "registry_helper.h"

class system_helper
{
	public:
		static std::string getComputerName();
		static std::string getBIOSManufacturer();
		static std::string getBIOSVersion();
		static std::string getSystemProductName();
		static std::string getVideoBiosVersion();
		static std::string getArchitectureString(const SYSTEM_INFO& sysInfo);
};

#endif //SYSTEM_UTILS_H
