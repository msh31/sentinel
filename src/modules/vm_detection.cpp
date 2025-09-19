//
// Created by Marco on 23/08/2025.
//

#include <sentinel/core/utils/system_helper.h>
#include <sentinel/modules/vm_detection.h>

//TODO: Improve accuracy by finding more details such as Disks, Networks etc..

bool vm_detection::isVirtualMachine() {
    checkForVMSignatures(system_helper::getComputerName(), "Computer Name");
    checkForVMSignatures(system_helper::getBIOSManufacturer(), "BIOS Manufacturer");
    checkForVMSignatures(system_helper::getBIOSVersion(), "BIOS Version");
    checkForVMSignatures(system_helper::getSystemProductName(), "System Product Name");
    checkForVMSignatures(system_helper::getVideoBiosVersion(), "Video BIOS Version");

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

    const std::vector<std::pair<std::string, std::vector<std::string>>> signatureGroups = {
        {"VMware", sigs.vmware},
        {"VirtualBox", sigs.virtualbox},
        {"Hyper-V", sigs.hyperv},
        {"Qemu", sigs.qemu}
    };

                        //streuctured bindings
    for (const auto& [name, keywords] : signatureGroups) {
        for (const auto& keyword : keywords) {
            if (value.find(keyword) != std::string::npos) {
                detectedSignatures.emplace_back(source + ": " + value);
                hypervisor = name;
                confidence = "HIGH";
                return name;
            }
        }
    }

    return "None";
}