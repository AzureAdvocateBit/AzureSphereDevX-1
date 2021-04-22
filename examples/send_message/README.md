# Azure IoT Hub Direct Methods and Azure IoT Central Commands

## Config app_manifest.json sample

1. Set ID Scope
1. Set Allowed connections
1. Set DeviceAuthentication


## Functions

```c
bool dx_azureIsConnected(void);
bool dx_azureMsgSend(const char* msg);
bool dx_azureMsgSendWithProperties(const char* msg, DX_MESSAGE_PROPERTY** messageProperties, size_t messagePropertyCount);
IOTHUB_DEVICE_CLIENT_LL_HANDLE dx_azureClientHandleGet(void);
void dx_azureConnectionStringSet(const char* connectionString); // Note, do not use Connection Strings for Production - this is here for lab workaround
void dx_azureInitialize(const char* idScope, const char* deviceTwinModelId);
void dx_azureToDeviceStop(void);
```


## Structures

```c
typedef struct DX_MESSAGE_PROPERTY
{
	const char* key;
	const char* value;
} DX_MESSAGE_PROPERTY;
```