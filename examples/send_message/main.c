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
#include "dx_azure_iot.h"
#include "dx_exit_codes.h"
#include "dx_terminate.h"
#include "dx_timer.h"
#include "dx_utilities.h"
#include <applibs/log.h>
#include <applibs/powermanagement.h>

#define JSON_MESSAGE_BYTES 256  // Number of bytes to allocate for the JSON telemetry message for IoT Central
static char msgBuffer[JSON_MESSAGE_BYTES] = { 0 };

// Message templates and property sets
static const char* msgTemplate = "{ \"Temperature\":%3.2f, \"Humidity\":%3.1f, \"Pressure\":%3.1f }";

static DX_MESSAGE_PROPERTY* telemetryMessageProperties[] = {
	&(DX_MESSAGE_PROPERTY) { .key = "appid", .value = "hvac" },
	&(DX_MESSAGE_PROPERTY) {.key = "format", .value = "json" },
	&(DX_MESSAGE_PROPERTY) {.key = "type", .value = "telemetry" },
	&(DX_MESSAGE_PROPERTY) {.key = "version", .value = "1" }
};


static void send_message_handler(EventLoopTimer* eventLoopTimer);
static void send_message_with_properties_handler(EventLoopTimer* eventLoopTimer);

DX_USER_CONFIG dx_config;

// Timers - both are set to trigger every 5 seconds
static DX_TIMER send_message_timer = { .period = { 5, 0 }, .name = "send_message_timer", .handler = send_message_handler };
static DX_TIMER send_message_with_properties_timer = { .period = { 5, 0 }, .name = "send_message_with_properties_timer", .handler = send_message_with_properties_handler };

// Add all timers by reference to this timer set
DX_TIMER* timerSet[] = { &send_message_handler, &send_message_with_properties_timer };

static void send_message_handler(EventLoopTimer* eventLoopTimer) {
	if (ConsumeEventLoopTimerEvent(eventLoopTimer) != 0) {
		dx_terminate(DX_ExitCode_ConsumeEventLoopTimeEvent);
		return;
	}
	// Create JSON message
	if (snprintf(msgBuffer, sizeof(msgBuffer), msgTemplate, 30.0, 60.0, 1010.0) > 0) {
		Log_Debug(msgBuffer);
		dx_azureMsgSend(msgBuffer);
	}
}

static void send_message_with_properties_handler(EventLoopTimer* eventLoopTimer) {
	if (ConsumeEventLoopTimerEvent(eventLoopTimer) != 0) {
		dx_terminate(DX_ExitCode_ConsumeEventLoopTimeEvent);
		return;
	}
	// Create JSON message
	if (snprintf(msgBuffer, sizeof(msgBuffer), msgTemplate, 30.0, 60.0, 1010.0) > 0) {
		Log_Debug(msgBuffer);
		dx_azureMsgSendWithProperties(msgBuffer, telemetryMessageProperties, NELEMS(telemetryMessageProperties));
	}
}

/// <summary>
///  Initialize peripherals, device twins, direct methods, timers.
/// </summary>
static void InitPeripheralsAndHandlers(void) {
	dx_azureInitialize(dx_config.scopeId, NULL);
	dx_timerSetStart(timerSet, NELEMS(timerSet));
}

/// <summary>
///     Close peripherals and handlers.
/// </summary>
static void ClosePeripheralsAndHandlers(void) {
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