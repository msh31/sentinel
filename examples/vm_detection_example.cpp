//
// Created by Marco on 24/08/2025.
//

#include <sentinel/modules/vm_detection.h>

int main()
{
	logger log;
	vm_detection vmDetect;

	bool isVM = vmDetect.isVirtualMachine();

	//defaults values are true
	log.consoleLoggingEnabled = true;
	log.fileLoggingEnabled = false;

	log.info("Starting analysis");
	if (isVM){
		log.warning("VM environment detected!");
		vmDetect.generateReport();
		return 0;
	}

	log.success("You are not in a Virtual Machine!");
	return 0;
}