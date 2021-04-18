#include "dx_timer.h"

static EventLoop* eventLoop = NULL;

EventLoop* dx_timerGetEventLoop(void) {
	if (eventLoop == NULL) {
		eventLoop = EventLoop_Create();
	}
	return eventLoop;
}

bool dx_timerChange(DX_TIMER* timer, const struct timespec* period) {
	if (timer->eventLoopTimer == NULL) { return false; }
	timer->period.tv_nsec = period->tv_nsec;
	timer->period.tv_sec = period->tv_sec;
	int result = SetEventLoopTimerPeriod(timer->eventLoopTimer, period);

	return result == 0 ? true : false;
}

bool dx_timerStart(DX_TIMER* timer) {
	EventLoop* eventLoop = dx_timerGetEventLoop();
	if (eventLoop == NULL) {
		return false;
	}

	if (timer->eventLoopTimer != NULL) {
		return true;
	}

	if (timer->period.tv_nsec == 0 && timer->period.tv_sec == 0) {  // Set up a disabled DX_TIMER for oneshot or change timer
		timer->eventLoopTimer = CreateEventLoopDisarmedTimer(eventLoop, timer->handler);
		if (timer->eventLoopTimer == NULL) {
			return false;
		}
	}
	else {
		timer->eventLoopTimer = CreateEventLoopPeriodicTimer(eventLoop, timer->handler, &timer->period);
		if (timer->eventLoopTimer == NULL) {
			return false;
		}
	}
	
	return true;
}

void dx_timerStop(DX_TIMER* timer) {
	if (timer->eventLoopTimer != NULL) {
		DisposeEventLoopTimer(timer->eventLoopTimer);
		timer->eventLoopTimer = NULL;
	}
}

void dx_timerSetStart(DX_TIMER* timerSet[], size_t timerCount) {
	for (int i = 0; i < timerCount; i++) {
		if (!dx_timerStart(timerSet[i])) {
			break;
		};
	}
}

void dx_timerSetStop(DX_TIMER* timerSet[], size_t timerCount) {
	for (int i = 0; i < timerCount; i++) {
		dx_timerStop(timerSet[i]);
	}
}

void dx_timerEventLoopStop(void) {
	EventLoop* eventLoop = dx_timerGetEventLoop();
	if (eventLoop != NULL) {
		EventLoop_Close(eventLoop);
	}
}

bool dx_timerOneShotSet(DX_TIMER* timer, const struct timespec* period) {
	EventLoop* eventLoop = dx_timerGetEventLoop();
	if (eventLoop == NULL) {
		return false;
	}

	if (timer->eventLoopTimer == NULL) {
		return false;
	}

	if (SetEventLoopTimerOneShot(timer->eventLoopTimer, period) != 0) {
		return false;
	}

	return true;
}