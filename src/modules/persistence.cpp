//
// Created by Marco on 26/08/2025.
//

#include <sentinel/modules/persistence.h>

PersistenceResult persistence::install() {
	PersistenceResult result;

	if (launchAtStartup) {
		HKEY targetHive = systemLevel ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER; // condition ? if_true : if_false
		result.registrySuccess = registry_helper::setRegistryValue(targetHive, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", startupFile, startupFilePath);

		if (result.registrySuccess) {
			result.details += "REG INSTALL OK ";
		} else {
			DWORD error = GetLastError();
			result.details += "REG INSTALL ERR (" + std::to_string(error) + ")";
		}

	}

	if (asService) {
		result.serviceSuccess = serviceHelper.createService(serviceName, serviceDisplayName, serviceExecutablePath);
		result.serviceSuccess += serviceHelper.startService(serviceName);

		if (result.serviceSuccess) {
			result.details += "SRV INSTALL OK ";
		} else {
			DWORD error = GetLastError();
			result.details += "SRV INSTALL ERR (" + std::to_string(error) + ")";
		}
	}

	return result;
}

PersistenceResult persistence::uninstall()
{
	PersistenceResult result;

	if (launchAtStartup){
		HKEY targetHive = systemLevel ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
		result.registrySuccess = registry_helper::removeRegistryValue(targetHive, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", startupFile);

		if (result.registrySuccess) {
			result.details += "REG DELETE OK ";
		} else {
			DWORD error = GetLastError();
			result.details += "REG DELETE ERR (" + std::to_string(error) + ")";

			if (error == ERROR_FILE_NOT_FOUND) {
				log.warning("Registry value '" + startupFile + "' was not found");
			} else if (error == ERROR_ACCESS_DENIED) {
				log.error("Access denied - need administrator privileges");
			}
		}
	}

	if (asService) {
		log.info("Attempting to stop and delete service: " + serviceName);

		bool stopSuccess = serviceHelper.stopService(serviceName);
		bool deleteSuccess = serviceHelper.deleteService(serviceName);

		result.serviceSuccess = stopSuccess && deleteSuccess;

		if (result.serviceSuccess) {
			result.details += "SRV DELETE OK";
		} else {
			result.details += "SRV DELETE ERR, REASON:  ";
			if (!stopSuccess) log.error("Failed to stop service");
			if (!deleteSuccess) log.error("Failed to delete service");
		}
	}

	return result;
}

PersistenceResult persistence::verify()
{
	PersistenceResult result;

	if (launchAtStartup) {
		HKEY targetHive = systemLevel ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
		std::string regValue = registry_helper::getRegistryValue(targetHive, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", startupFile);

		if (regValue == "Unknown") {
			DWORD error = GetLastError();
			result.registrySuccess = false;
			result.details += "REG VERIFY FAIL ( ";
			result.details += std::to_string(error) + " )";
		} else {
			result.registrySuccess = true;
			result.details += "REG VERIFY PASS";
		}
	}

	if (asService) {
		if (!serviceHelper.validateService(serviceName)) {
			DWORD error = GetLastError();

			result.serviceSuccess = false;
			result.details += "SRV VERIFY FAIL (";
			result.details += std::to_string(error) + " )";
		} else {
			result.serviceSuccess = true;
			result.details += "SRV VERIFY PASS";
		}
	}

	return result;
}