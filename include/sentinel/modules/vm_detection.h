//
// Created by Marco on 23/08/2025.
//

#ifndef VM_DETECTION_H
#define VM_DETECTION_H

#include <sentinel/core/utils/registry_helper.h>
#include <sentinel/core/logger.h>
#include <sentinel/common.h>

class vm_detection
{
	public:
		bool isVirtualMachine();
		void generateReport();

	private:
		static std::string getComputerName();
		static std::string getBIOSManufacturer();
		static std::string getBIOSVersion();
		static std::string getSystemProductName();
		static std::string getVideoBiosVersion();

		std::string checkForVMSignatures(const std::string& value, const std::string& source);

		std::string confidence = "LOW";
		std::string hypervisor = "None";

		std::vector<std::string> detectedSignatures = {};

		struct VMSignatures {
			std::vector<std::string> vmware = {"VMware", "VMware, Inc.", "VMW"};
			std::vector<std::string> virtualbox = {"VirtualBox", "innotek GmbH", "VBOX"};
			std::vector<std::string> hyperv = {"Microsoft Corporation", "Hyper-V"};
			std::vector<std::string> qemu = {"QEMU", "Bochs"};
		};

		registry_helper registryHelper;
		logger log;
};

#endif //VM_DETECTION_H
