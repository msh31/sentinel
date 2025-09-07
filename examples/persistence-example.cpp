//
// Created by Marco on 26/08/2025.
//

/*
 *		This example makes use of various parts of the SDK, such as:
 *
 *		- Service utilies
 *		- Registry utilities
 *		- Logger
 *
 */

#include <sentinel/modules/persistence.h>

int main()
{
	logger log;
	persistence persistence;

// config
	log.consoleLoggingEnabled = true;
	log.fileLoggingEnabled = true;

	persistence.asService = false;
	persistence.serviceName = "Example Name";
	persistence.serviceDisplayName = "Example Display Name";
	persistence.serviceExecutablePath = "Path\\To\\Executable"; // this NEEDS to be a service type executable, not any exe will work.

	persistence.launchAtStartup = true;
	persistence.systemLevel = true; // requires admin rights
	persistence.startupFile = "SentinelPersistenceExample";
	persistence.startupFilePath = "C:\\Windows\\System32\\calc.exe";

//usage
	log.info("Persistence Example");
	log.info("Installing persistence..");

	PersistenceResult result = persistence.install();

	if (result.onlyRegistry()) {
		log.success("Registry persistence installed: " + result.details);
	} else if (result.onlyService()) {
		log.success("Service persistence installed: " + result.details);
	} else if (result.bothSucceeded()) {
		log.success("Full persistence active: " + result.details);
	} else {
		log.error("Installation failed: " + result.details);
	}

	log.info("Verifying persistence..");
	PersistenceResult verificationResult = persistence.install();

	if (verificationResult.onlyRegistry()) {
		log.success("Registry persistence OK");
	} else if (verificationResult.onlyService()) {
		log.success("Service persistence OK");
	} else if (verificationResult.bothSucceeded()) {
		log.success("Persistence verified!: " + verificationResult.details);
	} else {
		log.error("Verification failed: " + verificationResult.details);
	}

	log.info("Removing persistence..");
	PersistenceResult removalResult = persistence.uninstall();

	if (removalResult.onlyRegistry()) {
		log.success("Registry persistence uninstalled: " + removalResult.details);
	} else if (removalResult.onlyService()) {
		log.success("Service persistence uninstalled: " + removalResult.details);
	} else if (removalResult.bothSucceeded()) {
		log.success("Persistence removal status: " + removalResult.details);
	} else {
		log.error("Removal failed: " + removalResult.details);
	}

	return 0;
}