/*
 *   DEVELOPER BOARD SELECTION
 *
 *   The following developer boards are supported.
 *
 *	   1. AVNET Azure Sphere Starter Kit.
 *     2. AVNET Azure Sphere Starter Kit Revision 2.
 *	   3. Seeed Studio Azure Sphere MT3620 Development Kit aka Reference Design Board or rdb.
 *	   4. Seeed Studio Seeed Studio MT3620 Mini Dev Board.
 *
 *   ENABLE YOUR DEVELOPER BOARD
 *
 *   Each Azure Sphere developer board manufacturer maps pins differently. You need to select the configuration that matches your board.
 *
 *   Follow these steps:
 *
 *	   1. Open CMakeLists.txt.
 *	   2. Uncomment the set command that matches your developer board.
 *	   3. Click File, then Save to save the CMakeLists.txt file which will auto generate the CMake Cache.
 */

#include "hw/azure_sphere_learning_path.h"   // Hardware definition

#include "dx_azure_iot.h"
#include "dx_config.h"
#include "dx_device_twins.h"
#include "dx_exit_codes.h"
#include "dx_terminate.h"
#include "dx_timer.h"
#include "dx_utilities.h"
#include <applibs/log.h>

#define JSON_MESSAGE_BYTES 256  // Number of bytes to allocate for the JSON telemetry message for IoT Central
static char msgBuffer[JSON_MESSAGE_BYTES] = { 0 };
DX_USER_CONFIG dx_config;

static void report_now_handler(EventLoopTimer* eventLoopTimer);
static void dt_desired_sample_rate_handler(DX_DEVICE_TWIN_BINDING* deviceTwinBinding);

// Update device twin every 5 seconds
static DX_TIMER report_now_timer = { .period = { 5, 0 }, .name = "report_now_timer", .handler = report_now_handler };

// Azure IoT Device Twins
static DX_DEVICE_TWIN_BINDING dt_desired_sample_rate = {
	.twinProperty = "DesiredSampleRate",
	.twinType = DX_TYPE_INT,
	.handler = dt_desired_sample_rate_handler };

static DX_DEVICE_TWIN_BINDING dt_reported_temperature = {
	.twinProperty = "ReportedCurrentTime",
	.twinType = DX_TYPE_STRING };

// Add all timers by reference to this timer set. 
// All timers in this set will be initialised in the InitPeripheralsAndHandlers function
DX_TIMER* timerSet[] = { &report_now_timer };

// Add all device twins by reference to the device twin 
// All device twins in this set will be initialised in the InitPeripheralsAndHandlers function
DX_DEVICE_TWIN_BINDING* deviceTwinBindingSet[] = { &dt_desired_sample_rate, &dt_reported_temperature };

static void report_now_handler(EventLoopTimer* eventLoopTimer) {
	if (ConsumeEventLoopTimerEvent(eventLoopTimer) != 0) {
		dx_terminate(DX_ExitCode_ConsumeEventLoopTimeEvent);
		return;
	}
	// get current UTC (Universal Time Coordinate) in ISO format
	dx_getCurrentUtc(msgBuffer, sizeof(msgBuffer));
	dx_deviceTwinReportState(&dt_reported_temperature, (void*)msgBuffer);
}

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

/// <summary>
///  Initialize peripherals, device twins, direct methods, timers.
/// </summary>
static void InitPeripheralsAndHandlers(void) {
	dx_azureInitialize(dx_config.scopeId, NULL);
	dx_timerSetStart(timerSet, NELEMS(timerSet));
	dx_deviceTwinSetOpen(deviceTwinBindingSet, NELEMS(deviceTwinBindingSet));
}

/// <summary>
///     Close peripherals and handlers.
/// </summary>
static void ClosePeripheralsAndHandlers(void) {
	dx_deviceTwinSetClose();
	dx_timerSetStop(timerSet, NELEMS(timerSet));
	dx_timerEventLoopStop();
}

int main(int argc, char* argv[]) {
	dx_registerTerminationHandler();

	dx_configParseCmdLineArguments(argc, argv, &dx_config);
	if (!dx_configValidate(&dx_config)) {
		return dx_getTerminationExitCode();
	}
	InitPeripheralsAndHandlers();

	// Main loop
	while (!dx_isTerminationRequired()) {
		int result = EventLoop_Run(dx_timerGetEventLoop(), -1, true);
		// Continue if interrupted by signal, e.g. due to breakpoint being set.
		if (result == -1 && errno != EINTR) {
			dx_terminate(DX_ExitCode_Main_EventLoopFail);
		}
	}

	ClosePeripheralsAndHandlers();
	Log_Debug("Application exiting.\n");
	return dx_getTerminationExitCode();
}