//
// Created by Marco on 23/08/2025.
//

#include <sentinel/core/utils/system_utils.h>
#include <sentinel/modules/vm_detection.h>

//TODO: Improve accuracy by finding more details such as Disks, Networks etc..

bool vm_detection::isVirtualMachine() {
    checkForVMSignatures(system_utils::getComputerName(), "Computer Name");
    checkForVMSignatures(system_utils::getBIOSManufacturer(), "BIOS Manufacturer");
    checkForVMSignatures(system_utils::getBIOSVersion(), "BIOS Version");
    checkForVMSignatures(system_utils::getSystemProductName(), "System Product Name");
    checkForVMSignatures(system_utils::getVideoBiosVersion(), "Video BIOS Version");

    return !detectedSignatures.empty();
}

void vm_detection::generateReport() {
    std::string hypervisor = vm_detection::hypervisor;

    log.info("Confidence: " + confidence);
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