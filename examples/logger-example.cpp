//
// Created by Marco on 23/08/2025.
//

#include <sentinel/core/logger.h>

int main()
{
	logger log;

	log.info("Starting security analysis");
	log.success("Process analysis module loaded");
	log.warning("Potential VM environment detected");
	log.error("Critical security threat found");
	log.fatal("System compromise detected");

	return 0;
}