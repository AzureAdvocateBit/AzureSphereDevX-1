#include "dx_config.h"

extern volatile sig_atomic_t terminationRequired;

// Usage text for command line arguments in application manifest.
static const char* cmdLineArgsUsageText =
"DPS connection type: \" CmdArgs \": [\"--ConnectionType\", \"DPS\", \"--ScopeID\", "
"\"<scope_id>\"]\n"
"Direction connection type: \" CmdArgs \": [\"--ConnectionType\", \"Direct\", "
"\"--ConnectionString\" \"]\n"
;

/// <summary>
///     Parse the command line arguments given in the application manifest.
/// </summary>
void dx_configParseCmdLineArguments(int argc, char* argv[], DX_USER_CONFIG* dx_config)
{
    int option = 0;
    static const struct option cmdLineOptions[] = {
        {"ConnectionType", required_argument, NULL, 'c'},
        {"ScopeID", required_argument, NULL, 's'},
        {"ConnectionString", required_argument, NULL, 'z'},
        {NULL, 0, NULL, 0} };

    // Loop over all of the options.
    while ((option = getopt_long(argc, argv, "c:s:r:z:", cmdLineOptions, NULL)) != -1) {
        // Check if arguments are missing. Every option requires an argument.
        if (optarg != NULL && optarg[0] == '-') {
            Log_Debug("WARNING: Option %c requires an argument\n", option);
            continue;
        }
        switch (option) {
        case 'c':
            Log_Debug("ConnectionType: %s\n", optarg);
            if (strcmp(optarg, "DPS") == 0) {
                dx_config->connectionType = ConnectionType_DPS;
            }
            else if (strcmp(optarg, "Direct") == 0) {
                dx_config->connectionType = ConnectionType_Direct;
            }
            break;
        case 's':
            Log_Debug("ScopeId: %s\n", optarg);
            dx_config->scopeId = optarg;
            break;
        case 'z':
            Log_Debug("Connection String: %s\n", optarg);
            dx_config->connectionString = optarg;
            break;
        default:
            // Unknown options are ignored.
            break;
        }
    }
}

/// <summary>
///     Validates if the values of the Scope ID, IotHub Hostname and Device ID were set.
/// </summary>
/// <returns>DX_ExitCode_Success if the parameters were provided; otherwise another
/// ExitCode value which indicates the specific failure.</returns>
bool dx_configValidate(DX_USER_CONFIG* dx_config)
{
    ExitCode validationExitCode = DX_ExitCode_Success;

    if (dx_config->connectionType < ConnectionType_DPS || dx_config->connectionType > ConnectionType_Direct) {
        dx_terminate(DX_ExitCode_Validate_ConnectionType);
    }

    if (dx_config->connectionType == ConnectionType_DPS) {
        if (dx_config->scopeId == NULL) {
            validationExitCode = DX_ExitCode_Validate_ScopeId;
        }
        else {
            Log_Debug("Using DPS Connection: Azure IoT DPS Scope ID %s\n", dx_config->scopeId);
        }
    }

    if (dx_config->connectionType == ConnectionType_Direct) {
        if (dx_config->connectionString == NULL) {
            dx_terminate(DX_ExitCode_Validate_IotHubConnectionString);
        }
        if (validationExitCode == DX_ExitCode_Success) {
            Log_Debug("Using Direct Connection: Azure IoT Hub %s\n", dx_config->connectionString);
        }
    }

    if (validationExitCode != DX_ExitCode_Success) {
        Log_Debug("Command line arguments for application shoud be set as below\n%s",
            cmdLineArgsUsageText);
    }

    return dx_getTerminationExitCode() == DX_ExitCode_Success;
}