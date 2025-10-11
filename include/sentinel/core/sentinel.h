//
// Created by Marco on 22/08/2025.
//

#ifndef SENTINEL_H
#define SENTINEL_H

// core functionality
#include <sentinel/core/logger.h>
#include <sentinel/common.h>
#include <sentinel/core/config.h> //not required right now

// core utilities
#ifdef _WIN32 // TODO: improve this situation
#include <sentinel/core/utils/system_helper.h>
#include <sentinel/core/utils/registry_helper.h>
#include <sentinel/core/utils/service_helper.h>
#include <sentinel/core/utils/process_helper.h>
#endif

// TODO: determine popular modules

#endif //SENTINEL_H