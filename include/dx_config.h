#pragma once

#include "dx_terminate.h"
#include <applibs/log.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

/// <summary>
/// Connection types to use when connecting to the Azure IoT Hub.
/// </summary>
typedef enum {
    ConnectionType_NotDefined = 0,
    ConnectionType_DPS = 1,
    ConnectionType_Direct = 2
} ConnectionType;

typedef struct {
    const char* scopeId;                                      // ID Scope for DPS.
    const char* connectionString;                             // Fallback to Connection string (not recommended)
    ConnectionType connectionType;
} DX_USER_CONFIG;



void dx_configParseCmdLineArguments(int argc, char* argv[], DX_USER_CONFIG *dx_config);
bool dx_configValidate(DX_USER_CONFIG *dx_config);
