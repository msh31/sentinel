//
// Created by Marco on 28/08/2025.
//

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <sentinel/core/utils/service_helper.h>
#include <sentinel/common.h>
#include <sentinel/core/logger.h>
#include <sentinel/core/utils/registry_helper.h>

struct PersistenceResult {
	bool registrySuccess = false;
	bool serviceSuccess = false;
	std::string details = "";

	bool bothSucceeded() const
	{
		return registrySuccess && serviceSuccess;
	}

	bool onlyRegistry() const
	{
		return registrySuccess && !serviceSuccess;
	}

	bool onlyService() const
	{
		return serviceSuccess && !registrySuccess;
	}
};

class persistence
{
	public:
		std::string serviceName, serviceDisplayName, serviceExecutablePath;
		std::string startupFile, startupFilePath;

		bool asService, launchAtStartup, systemLevel;
		PersistenceResult install();
		PersistenceResult uninstall();
		PersistenceResult verify();

	private:
		service_helper serviceHelper;
		logger log;
};

#endif //PERSISTENCE_H
