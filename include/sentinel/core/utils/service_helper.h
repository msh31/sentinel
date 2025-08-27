//
// Created by Marco on 28/08/2025.
//

#ifndef SERVICE_HELPER_H
#define SERVICE_HELPER_H

#include <sentinel/core/logger.h>
#include <sentinel/common.h>

class service_helper
{
	public:
		logger log;

		//TODO: improve by requiring less pparameters. maybe not make them static and use logger from a membber var as a state object we need to manage between functions?
		bool createService(const std::string& serviceName, const std::string& displayName, const std::string& executablePath);
		bool startService(const std::string& serviceName);
		bool stopService(const std::string& serviceName);
		bool deleteService(const std::string& serviceName);
		bool validateService(const std::string& serviceName);
};



#endif //SERVICE_HELPER_H
