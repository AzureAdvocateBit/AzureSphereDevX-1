#pragma once

/// <summary>
/// Exit codes for this application. These are used for the
/// application exit code.  They must all be between zero and 255,
/// where zero is reserved for successful termination.
/// </summary>
typedef enum {
	DX_ExitCode_Success = 0,
	DX_ExitCode_TermHandler_SigTerm = 1,
	DX_ExitCode_Main_EventLoopFail = 2,
	DX_ExitCode_Missing_ID_Scope = 3,

	DX_ExitCode_Open_Peripheral = 10,
	DX_ExitCode_OpenDeviceTwin = 11,
	DX_ExitCode_AzureCloudToDeviceHandler = 12,
	DX_ExitCode_InterCoreHandler = 13,
	DX_ExitCode_ConsumeEventLoopTimeEvent = 14,
	DX_ExitCode_Gpio_Read = 15,
	DX_ExitCode_InterCoreReceiveFailed = 16,

	DX_ExitCode_IsButtonPressed = 20,
	DX_ExitCode_ButtonPressCheckHandler = 21,
	DX_ExitCode_Led2OffHandler = 22,

	DX_ExitCode_MissingRealTimeComponentId = 23,

	DX_ExitCode_Validate_ConnectionType = 30,
    DX_ExitCode_Validate_ScopeId = 31,
    DX_ExitCode_Validate_IotHubConnectionString = 32,

	DX_ExitCode_Gpio_Not_Initialized = 33,
	DX_ExitCode_Gpio_Wrong_Direction = 34,
	DX_ExitCode_Gpio_Open_Output_Failed = 35,
	DX_ExitCode_Gpio_Open_Input_Failed = 36,
	DX_ExitCode_Gpio_Open_Direction_Unknown = 37

} ExitCode;