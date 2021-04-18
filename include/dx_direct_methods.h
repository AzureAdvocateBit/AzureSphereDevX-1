#pragma once

#include "dx_azure_iot.h"
#include "dx_gpio.h"

typedef enum 
{
	DX_METHOD_SUCCEEDED = 200,
	DX_METHOD_FAILED = 500,
	DX_METHOD_NOT_FOUND = 404
} DX_DIRECT_METHOD_RESPONSE_CODE;

struct _directMethodBinding {
	const char* methodName;
	DX_DIRECT_METHOD_RESPONSE_CODE(*handler)(JSON_Value* json, struct _directMethodBinding* peripheral, char** responseMsg);
};

typedef struct _directMethodBinding DX_DIRECT_METHOD_BINDING;

void dx_directMethodSetClose(void);
void dx_directMethodSetOpen(DX_DIRECT_METHOD_BINDING* directMethods[], size_t directMethodCount);
int dx_directMethodHandler(const char* method_name, const unsigned char* payload, size_t payloadSize,
	unsigned char** responsePayload, size_t* responsePayloadSize, void* userContextCallback);
