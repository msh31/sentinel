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
			result.details += "Registry: OK ";
		} else {
			result.details += "Registry: ERR ";
		}

	}

	if (asService) {
		result.serviceSuccess = serviceHelper.createService(serviceName, serviceDisplayName, serviceExecutablePath);
		result.serviceSuccess += serviceHelper.startService(serviceName);

		if (result.serviceSuccess) {
			result.details += "Service: OK ";
		} else {
			result.details += "Service: ERR ";
		}
	}

	return result;
}

PersistenceResult persistence::uninstall()
{
	PersistenceResult result;

	if (launchAtStartup){

	}


	return result; //placeholder
}

PersistenceResult persistence::verify()
{
	PersistenceResult result;
	return result; //placeholder
}
