//
// Created by Marco on 23/08/2025.
//

#include <sentinel/modules/vm_detection.h>

bool vm_detection::isVirtualMachine() {
    checkForVMSignatures(getComputerName(), "Computer Name");
    checkForVMSignatures(getBIOSManufacturer(), "BIOS Manufacturer");
    checkForVMSignatures(getBIOSVersion(), "BIOS Version");
    checkForVMSignatures(getSystemProductName(), "System Product Name");
    checkForVMSignatures(getVideoBiosVersion(), "Video BIOS Version");

    return !detectedSignatures.empty();
}

void vm_detection::generateReport() {
    std::string hypervisor = vm_detection::hypervisor;

    log.info("Confidence of being a VM: " + confidence);
    log.info("\nDetected Signatures: \n");

    for(const auto &sigs : detectedSignatures) {
        if(sigs.empty()) {
            log.info("No signatures found!");
        } else {
            log.warning("Found signatures:\n" + sigs);
        }
    }

    log.info("Detected HyperVisor: " + vm_detection::hypervisor);
}

std::string vm_detection::checkForVMSignatures(const std::string& value, const std::string& source) {
    const VMSignatures sigs;

    for (const auto& keyword : sigs.vmware) {
        if (value.find(keyword) != std::string::npos) {
            detectedSignatures.emplace_back(source + ": " + value);
            hypervisor = "VMware";
            confidence = "HIGH";
            return "VMware";
        }
    }

    for (const auto& keyword : sigs.virtualbox) {
        if (value.find(keyword) != std::string::npos) {
            detectedSignatures.emplace_back(source + ": " + value);
            hypervisor = "VirtualBox";
            confidence = "HIGH";
            return "VirtualBox";
        }
    }

    for (const auto& keyword : sigs.hyperv) {
        if (value.find(keyword) != std::string::npos) {
            detectedSignatures.emplace_back(source + ": " + value);
            hypervisor = "Hyper-V";
            confidence = "HIGH";
            return "Hyper-V";
        }
    }

    for (const auto& keyword : sigs.qemu) {
        if (value.find(keyword) != std::string::npos) {
            detectedSignatures.emplace_back(source + ": " + value);
            hypervisor = "Qemu";
            confidence = "HIGH";
            return "Qemu";
        }
    }

    return "None";
}

std::string vm_detection::getComputerName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);

    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    }

    return "Unknown";
}

std::string vm_detection::getSystemProductName() {
    return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "SystemProductName");
}

std::string vm_detection::getBIOSManufacturer() {
    return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "SystemManufacturer");
}
std::string vm_detection::getBIOSVersion() {
    return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSVersion");
}

std::string vm_detection::getVideoBiosVersion() {
    return registry_helper::getRegistryValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "VideoBiosVersion");
}