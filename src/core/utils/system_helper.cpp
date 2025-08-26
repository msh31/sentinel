//
// Created by Marco on 25/08/2025.
//

#include <sentinel/core/utils/system_helper.h>
#include <sentinel/core/utils/registry_helper.h>
#include <sentinel/core/logger.h>
#include <sentinel/common.h>

std::string system_helper::getComputerName() {
	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName);

	if (GetComputerNameA(computerName, &size)) {
		return std::string(computerName); // NOLINT(*-return-braced-init-list)
	}

	return "Unknown";
}
std::string system_helper::getSystemProductName() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "SystemProductName");
}
std::string system_helper::getBIOSManufacturer() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "SystemManufacturer");
}
std::string system_helper::getBIOSVersion() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "BIOSVersion");
}
std::string system_helper::getVideoBiosVersion() {
	return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", "VideoBiosVersion");
}
std::string system_helper::getArchitectureString(const SYSTEM_INFO& sysInfo){
	// for reference, it's labeled as AMD64 simply because AMD got there first, Intel CPU's these days use it too, before that it was x86

	switch (sysInfo.wProcessorArchitecture) { //this is a WORD value which is an 16-bit unsigned integer meaniing it can be used in a switch statement, apparently thats a thiing
		case PROCESSOR_ARCHITECTURE_AMD64:
			return std::string("x64");
		case PROCESSOR_ARCHITECTURE_ARM:
			return std::string("ARM");
		case PROCESSOR_ARCHITECTURE_ARM64:
			return std::string("ARM64");
		case PROCESSOR_ARCHITECTURE_INTEL:
			return std::string("x86");
	}

	return "Unknown";
}

std::vector<std::string> system_helper::filterFilesByType(const std::vector<std::string>& files, const std::vector<std::string>& extensions)
{
	std::vector<std::string> filteredFiles;

	for (const auto& file : files) {
		std::filesystem::path filePath = file;
		std::string extension = filePath.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

		for (const auto& targetExt : extensions) {
			std::string lowerTargetExt = targetExt;
			std::transform(lowerTargetExt.begin(), lowerTargetExt.end(), lowerTargetExt.begin(), ::tolower);

			if (extension == lowerTargetExt) {
				filteredFiles.push_back(file);
				break;
			}
		}
	}

	return filteredFiles;
}

bool system_helper::createService(const std::string& serviceName, const std::string& displayName, const std::string& executablePath, logger& log)
{
	SC_HANDLE scManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);

	if (!scManager) {
		return false;
	}

	SC_HANDLE service = CreateServiceA(scManager, serviceName.c_str(), displayName.c_str(), SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, executablePath.c_str(),
		nullptr, nullptr, nullptr, nullptr, nullptr
	);

	if (!service) {
		if (GetLastError() == 1073) {
			log.error("Service already exists.");
		}
		else {
			log.error("error code: " + std::to_string(GetLastError()));
		}

		CloseServiceHandle(scManager);
		return false;
	}

	log.info("Service created.");
	CloseServiceHandle(service);
	CloseServiceHandle(scManager);
	return true;
}

bool system_helper::startService(const std::string& serviceName, logger& log)
{
	SC_HANDLE scManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);

	if (!scManager) {
		log.error("Failed to open Service Control Manager. Error: " + std::to_string(GetLastError()));
		return false;
	}

	SC_HANDLE service = OpenServiceA(scManager, serviceName.c_str(), SERVICE_START);

	if (!service) {
		if (GetLastError() == 1053) {
			log.error("Invalid service executable provided!");
		}
		else {
			log.error("error code: " + std::to_string(GetLastError()));
		}

		CloseServiceHandle(scManager);
		return false;
	}

	bool success = StartServiceA(service, 0, nullptr);

	if (!success) {
		log.error("Failed to start service. Error: " + std::to_string(GetLastError()));
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scManager);
	return success;
}

bool system_helper::stopService(const std::string& serviceName, logger& log)
{
	SC_HANDLE scManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
	SERVICE_STATUS serviceStatus;

	if (!scManager) {
		log.error("Failed to open Service Control Manager. Error: " + std::to_string(GetLastError()));
		return false;
	}

	SC_HANDLE service = OpenServiceA(scManager, serviceName.c_str(), SERVICE_STOP);

	if (!service) {
		log.error("error code: " + std::to_string(GetLastError()));
		CloseServiceHandle(scManager);
		return false;
	}

	bool success = ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus);

	if (!success) {
		log.warning("Failed to stop service. Error: " + std::to_string(GetLastError()));
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scManager);
	return success;
}

bool system_helper::deleteService(const std::string& serviceName, logger& log)
{
	SC_HANDLE scManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
	SERVICE_STATUS serviceStatus;

	if (!scManager) {
		log.error("Failed to open Service Control Manager. Error: " + std::to_string(GetLastError()));
		return false;
	}

	SC_HANDLE service = OpenServiceA(scManager, serviceName.c_str(), DELETE | SERVICE_STOP);

	if (!service) {
		log.error("error code: " + std::to_string(GetLastError()));
		CloseServiceHandle(scManager);
		return false;
	}

	if (!ControlService(service, SERVICE_CONTROL_STOP, &serviceStatus)) {
		log.warning("Failed to stop service before deletion. Error: " + std::to_string(GetLastError()));
	}

	bool success = DeleteService(service);

	CloseServiceHandle(service);
	CloseServiceHandle(scManager);
	return success;
}

bool system_helper::validateService(const std::string& serviceName, logger& log)
{
	SC_HANDLE scManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);

	if (!scManager) {
		log.error("Failed to open Service Control Manager. Error: " + std::to_string(GetLastError()));
		return false;
	}

	SC_HANDLE service = OpenServiceA(scManager, serviceName.c_str(), SERVICE_QUERY_STATUS);

	bool exists = (service != nullptr);

	if (service != nullptr) {
		CloseServiceHandle(service);
	}

	CloseServiceHandle(scManager);
	return exists;
}