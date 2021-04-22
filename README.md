# Azure Sphere DevX library

The DevX library can accelerate your development and improve your developer experience with Azure Sphere. The DevX library addresses many common Azure Sphere scenarios, it will help reduce the amount of code you write and improve readability and long term application maintenance.

The DevX library is built from the Azure Sphere samples and aims to facilitate Azure Sphere best practices. The DevX library is light weight, addresses common scenarios, and will sit alongside your existing code base.

The DevX library design is context based, you declare a context and there are libraries that support that context. See the [Encapsulate Pattern](https://accu.org/journals/overload/12/63/kelly_246/), it's a fair description of how this library works.

The library prefixes all file names, functions, structures, and enums with DX_ or dx_ to avoid clashes with existing code and file names.

The library supports the following contexts:

1. Timers.
1. Device Twins.
1. Direct Methods.
1. Intercore communications.
1. GPIO. 

     Note, GPIO is only the peripheral class supported as it's use can be generalized. No attempt will be made to generalize the use of I2C and SPI given the highly varied nature of their use.
 
1. Azure IoT messaging.

## Examples

The following is an example of how you can declare a number of device twins context objects.

```c
static DX_DEVICE_TWIN_BINDING dt_desired_sample_rate = {
	.twinProperty = "DesiredSampleRate",
	.twinType = DX_TYPE_INT,
	.handler = dt_desired_sample_rate_handler };
```

```c
static DX_DEVICE_TWIN_BINDING dt_reported_temperature = {
	.twinProperty = "ReportedCurrentTime",
	.twinType = DX_TYPE_STRING };
```

You will see that the declarations include the property name, the type, and the handler function to call when a device twin update for the property is received.

The device twins need to be initialized, but first they need to be added to an array or set of device twins that are added by reference.

```c
DX_DEVICE_TWIN_BINDING* deviceTwinBindingSet[] = { &dt_desired_sample_rate, &dt_reported_temperature };
```

and finally the array of twins needs to be opened.

```c
dx_timerSetStart(timerSet, NELEMS(timerSet));
```

When a device twin message is received, this set of device twins is checked for a matching property name, when a match is found, the payload JSON is deserialized, the type is checked to ensure it is of the correct type, and then the handler is called and the device twin context is passed to the handler function.

The following is an example of the device twin handler that is called.

```c
static void dt_desired_sample_rate_handler(DX_DEVICE_TWIN_BINDING* deviceTwinBinding) {
	// validate data is sensible range before applying
	if (deviceTwinBinding->twinType == DX_TYPE_INT && *(int*)deviceTwinBinding->twinState >= 0 && *(int*)deviceTwinBinding->twinState <= 120) {
		dx_timerChange(&report_now_timer, &(struct timespec){*(int*)deviceTwinBinding->twinState, 0});
		dx_deviceTwinAckDesiredState(deviceTwinBinding, deviceTwinBinding->twinState, DX_DEVICE_TWIN_COMPLETED);
	} else {
		dx_deviceTwinAckDesiredState(deviceTwinBinding, deviceTwinBinding->twinState, DX_DEVICE_TWIN_ERROR);
	}

	/*	Casting device twin state examples

		float value = *(float*)deviceTwinBinding->twinState;
		int value = *(int*)deviceTwinBinding->twinState;
		bool value = *(bool*)deviceTwinBinding->twinState;
		char* value = (char*)deviceTwinBinding->twinState;
	*/
}
```

This is how the context model works. You declare the object and you can by reference to a function that understands the context object.

## Further examples

Here are some examples of other contexts you can declare.

### Timer context

```c
DX_TIMER report_now_timer = { .period = { 5, 0 }, .name = "report_now_timer", .handler = report_now_handler };
```

### GPIO context

```c
DX_TIMER blinkLedTimer = { .period = { 0, 500 * oneMS }, .name = "blinkLedTimer", .handler = BlinkLedHandler };
```

### Direct methods

```c
DX_DIRECT_METHOD_RESPONSE_CODE LightControlHandler(JSON_Value* json, DX_DIRECT_METHOD_BINDING* directMethodBinding, char** responseMsg);
```

### Intercore messages

```c
INTERCORE_CONTEXT intercore_environment_ctx = {
	.sockFd = -1, .nonblocking_io = true,
	.rtAppComponentId = CORE_ENVIRONMENT_COMPONENT_ID, .interCoreCallback = intercore_environment_receive_msg_handler,
	.intercore_recv_block = &intercore_recv_block, .intercore_recv_block_length = sizeof(intercore_recv_block)
};
```
