//
// Created by Marco on 28/08/2025.
//

#include <sentinel/core/utils/service_helper.h>

bool service_helper::createService(const std::string& serviceName, const std::string& displayName, const std::string& executablePath) {
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

bool service_helper::startService(const std::string& serviceName) {
	SC_HANDLE scManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);

	if (!scManager) {
		log.error("Failed to open Service Control Manager. Error: " + std::to_string(GetLastError()));
		return false;
	}

	SC_HANDLE service = OpenServiceA(scManager, serviceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS);

	if (!service) {
		DWORD err = GetLastError();
		if (err == 1053) {
			log.error("Invalid service executable provided!");
		} else if (err == 1056) {
			log.error("An instance of this service is already running.");
		} else if (err == 1060) {
			log.error("The specified service does not exist as an installed service.");
		} else {
			log.error("error code: " + std::to_string(err));
		}
		CloseServiceHandle(scManager);
		return false;
	}

	SERVICE_STATUS status;
	if (QueryServiceStatus(service, &status) && status.dwCurrentState == SERVICE_RUNNING) {
		log.info("Service already running.");
		CloseServiceHandle(service);
		CloseServiceHandle(scManager);
		return true;  // sure....
	}

	bool success = StartServiceA(service, 0, nullptr);

	if (!success) {
		DWORD err = GetLastError();
		log.error("Failed to start service. Error: " + std::to_string(err));
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scManager);
	return success;
}

bool service_helper::stopService(const std::string& serviceName) {
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

bool service_helper::deleteService(const std::string& serviceName) {
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

bool service_helper::validateService(const std::string& serviceName) {
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