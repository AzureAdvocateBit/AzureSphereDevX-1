# Azure Sphere DevX library

The DevX library can accelerate your development and improve your developer experience with Azure Sphere. The DevX library addresses many common Azure Sphere scenarios, it will help reduce the amount of code you write and improve readability and long-term application maintenance.

To learn more about Azure Sphere and Azure RTOS check out [Combining Azure Sphere IoT security with Azure RTOS real-time capabilities](https://techcommunity.microsoft.com/t5/internet-of-things/combining-azure-sphere-iot-security-with-azure-rtos-real-time/ba-p/1992869?WT.mc_id=iot-0000-dglover) article.

There are two Microsoft Learn modules which include hands-on labs you can download to start your Azure Sphere and Azure RTOS journey.

- [Develop secure IoT solutions for Azure Sphere, Azure RTOS and Azure IoT Central](https://docs.microsoft.com/en-us/learn/modules/develop-secure-iot-solutions-azure-sphere-iot-central?WT.mc_id=iot-10976-dglover)
- [Develop secure IoT Solutions for Azure Sphere, Azure RTOS and IoT Hub](https://docs.microsoft.com/en-us/learn/modules/develop-secure-iot-solutions-azure-sphere-iot-hub?WT.mc_id=iot-11691-dglover)

The DevX library is built from the [Azure Sphere samples](https://github.com/Azure/azure-sphere-samples), it's well tested, and aims to facilitate Azure Sphere best practices. The DevX library is lightweight, addresses common scenarios, and will sit alongside your existing code base.

The DevX library design is context-based, you declare a context and the libraries supports the context. See the [Encapsulate Pattern](https://accu.org/journals/overload/12/63/kelly_246/), it's a fair description of how this library works.

The library prefixes all file names, functions, structures, and enums with DX_ or dx_ to avoid clashes with existing code and file names.

The library supports the following contexts:

Note, you will find examples of each context in the examples folder of this repo or you can click on the context type to navigate to an example in your web browser.

1. [Azure IoT messaging](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/send_message).
1. [Direct Methods](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/direct_methods).
1. [Device Twins](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/device_twins).
1. [GPIO](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/gpio_example). Note, GPIO is supported as it's use can be generalized. There are no plans to generalize the use of ADC, PWM, I2C and SPI peripherals given the varied nature of their use.
1. [Intercore communications](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/intercore_example).
1. [Termination](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/terminate_example).
1. [Timers](https://github.com/gloveboxes/AzureSphereDevX/tree/main/examples/timer_example).
 


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

Next the each device twins needs to be added by reference to an array (or set) of device twins.

```c
DX_DEVICE_TWIN_BINDING* deviceTwinBindingSet[] = { &dt_desired_sample_rate, &dt_reported_temperature };
```

and finally the array of device twins needs to be initialized or started.

```c
dx_timerSetStart(timerSet, NELEMS(timerSet));
```

When a device twin message is received, this set of device twins is checked for a matching property name, when a match is found, the JSON payload is deserialized, the type is checked to ensure it is of the correct type as declared in the device twin context. Then the device twin context handler is called passing the context by reference to the handler function.

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

This is how the context model works. You declare the object and you pass by reference to a function that understands the context object.  You don't have to deal with all the underlying code for managing the device twin callback, the JSON deserialisation, or the type checking. That is all done for you, the function is called, and you have access to the context to make further decisions in your code.

## Further examples

Here are some examples of other contexts you can declare.

### Timer context

```c
DX_TIMER report_now_timer = { .period = { 5, 0 }, .name = "report_now_timer", .handler = report_now_handler };
```

### GPIO context

```c
DX_GPIO buttonA = {.pin = BUTTON_A, .name = "buttonA",.direction = DX_INPUT, .detect = DX_GPIO_DETECT_LOW };
```

### Direct method binding

```c
DX_DIRECT_METHOD_BINDING dm_restartDevice = { .methodName = "RestartDevice", .handler = RestartDeviceHandler };
```

### Intercore messages

```c
INTERCORE_CONTEXT intercore_environment_ctx = {
	.sockFd = -1, .nonblocking_io = true,
	.rtAppComponentId = CORE_ENVIRONMENT_COMPONENT_ID, .interCoreCallback = intercore_environment_receive_msg_handler,
	.intercore_recv_block = &intercore_recv_block, .intercore_recv_block_length = sizeof(intercore_recv_block)
};
```

## How to use the library

This example assumes you are using git, otherwise download the [Azure Sphere DevX library](https://github.com/gloveboxes/AzureSphereDevX) and add to the directory to your project.

```bash
git submodule add https://github.com/gloveboxes/AzureSphereDevX.git
```

### Update your CMakeLists.txt to include the DevX library

add the library sub directory.

```text
add_subdirectory("AzureSphereDevX" out)
```

add *azure_sphere_devx* to the link libraries.

```text
target_link_libraries (${PROJECT_NAME} applibs pthread gcc_s c azure_sphere_devx)
```

add to include directories

```text
target_include_directories(${PROJECT_NAME} PUBLIC AzureSphereDevX/include )
```

The following is an example of what the completed CMakeLists.txt file could look like.

```text
cmake_minimum_required (VERSION 3.10)
project (AzureSphereAzureIoT C)

azsphere_configure_tools(TOOLS_REVISION "21.01")
azsphere_configure_api(TARGET_API_SET "8")

add_subdirectory("AzureSphereDevX" out)

set(Source
    "main.c"
)
source_group("Source" FILES ${Source})

set(ALL_FILES
    ${Source}
)

# Create executable
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_compile_definitions(${PROJECT_NAME} PUBLIC AZURE_IOT_HUB_CONFIGURED)
target_link_libraries(${PROJECT_NAME} applibs pthread gcc_s c azure_sphere_devx )

target_include_directories(${PROJECT_NAME} PUBLIC AzureSphereDevX/include . )

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-unknown-pragmas)

azsphere_target_add_image_package(${PROJECT_NAME})
```

