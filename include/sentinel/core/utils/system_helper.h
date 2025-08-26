//
// Created by Marco on 25/08/2025.
//

#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <sentinel/common.h>
#include <sentinel/core/logger.h>

class system_helper
{
	public:
		static std::string getComputerName();
		static std::string getBIOSManufacturer();
		static std::string getBIOSVersion();
		static std::string getSystemProductName();
		static std::string getVideoBiosVersion();
		static std::string getArchitectureString(const SYSTEM_INFO& sysInfo);

		std::vector<std::string> filterFilesByType(const std::vector<std::string>& files, const std::vector<std::string>& extensions);

		//TODO: improve by requiring less pparameters. maybe not make them static and use logger from a membber var as a state object we need to manage between functions?
		static bool createService(const std::string& serviceName, const std::string& displayName, const std::string& executablePath, logger& log);
		static bool startService(const std::string& serviceName, logger& log);
		static bool stopService(const std::string& serviceName, logger& log);
		static bool deleteService(const std::string& serviceName, logger& log);
		static bool validateService(const std::string& serviceName, logger& log);
};

#endif //SYSTEM_UTILS_H
