# Application termination

## Enums

The following exit code enums are defined in the dx_exit_codes.h file.

```c
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
```


## Functions

```c
void dx_registerTerminationHandler(void);
void dx_terminationHandler(int signalNumber);
void dx_terminate(int exitCode);
bool dx_isTerminationRequired(void);
int dx_getTerminationExitCode(void);
```