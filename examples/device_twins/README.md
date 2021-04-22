# Azure IoT Hub Direct Methods and Azure IoT Central Commands

## Config app_manifest.json sample

1. Set ID Scope
1. Set Allowed connections
1. Set DeviceAuthentication

## Enums

Device twin types

```c
typedef enum {
	DX_TYPE_UNKNOWN = 0,
	DX_TYPE_BOOL = 1,
	DX_TYPE_FLOAT = 2,
	DX_TYPE_INT = 3,
	DX_TYPE_STRING = 4
} DX_DEVICE_TWIN_TYPE;
```

Device twin acknowledgment codes

```c
typedef enum
{
	DX_DEVICE_TWIN_COMPLETED = 200,
	DX_DEVICE_TWIN_ERROR = 500,
	DX_DEVICE_TWIN_INVALID = 404
} DX_DEVICE_TWIN_RESPONSE_CODE;
```


## Functions

```c
bool dx_deviceTwinAckDesiredState(DX_DEVICE_TWIN_BINDING* deviceTwinBinding, void* state, DX_DEVICE_TWIN_RESPONSE_CODE statusCode);
bool dx_deviceTwinReportState(DX_DEVICE_TWIN_BINDING* deviceTwinBinding, void* state);
void dx_deviceTwinClose(DX_DEVICE_TWIN_BINDING* deviceTwinBinding);
void dx_deviceTwinOpen(DX_DEVICE_TWIN_BINDING* deviceTwinBinding);
void dx_deviceTwinSetClose(void);
void dx_deviceTwinSetOpen(DX_DEVICE_TWIN_BINDING* deviceTwins[], size_t deviceTwinCount);
void dx_deviceTwinsReportStatusCallback(int result, void* context);
void dx_twinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char* payload, size_t payloadSize, void* userContextCallback);
```


## Structures

Device twin context structure.

```c
struct _deviceTwinBinding {
	const char* twinProperty;
	void* twinState;
	int twinVersion;
	bool twinStateUpdated;
	DX_DEVICE_TWIN_TYPE twinType;
	void (*handler)(struct _deviceTwinBinding* deviceTwinBinding);
};
```