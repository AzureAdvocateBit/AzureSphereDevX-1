#pragma once

#include "azure_prov_client/prov_client_const.h"
#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/prov_transport.h"
#include "azure_prov_client/prov_transport_mqtt_client.h"
#include "dx_device_twins.h"
#include "dx_direct_methods.h"
#include "dx_terminate.h"
#include "dx_timer.h"
#include "dx_utilities.h"
#include "iothubtransportmqtt.h"
#include <applibs/log.h>
#include <azure_sphere_provisioning.h>
#include <errno.h>
#include <iothub_client_options.h>
#include <iothub_device_client_ll.h>
#include <iothubtransportmqtt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct DX_MESSAGE_PROPERTY
{
	const char* key;
	const char* value;
} DX_MESSAGE_PROPERTY;

bool dx_azureIsConnected(void);
bool dx_azureMsgSend(const char* msg);
bool dx_azureMsgSendWithProperties(const char* msg, DX_MESSAGE_PROPERTY** messageProperties, size_t messagePropertyCount);
IOTHUB_DEVICE_CLIENT_LL_HANDLE dx_azureClientHandleGet(void);
void dx_azureConnectionStringSet(const char* connectionString); // Note, do not use Connection Strings for Production - this is here for lab workaround
void dx_azureInitialize(const char* idScope, const char* deviceTwinModelId);
void dx_azureToDeviceStop(void);
