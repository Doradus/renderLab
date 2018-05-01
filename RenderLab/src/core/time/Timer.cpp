#include "Timer.h"

Timer::Timer() :
currentTime (0),
prevTime(0),
isStopped(true),
isPaused(false) {
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*) &countsPerSecond);
	secondsPerTick = 1.0 / (double) countsPerSecond;
}

Timer::~Timer() {
}

void Timer::Tick() {
	QueryPerformanceCounter((LARGE_INTEGER*) &currentTime);

	deltaTime = (float)((currentTime - prevTime) * secondsPerTick);

	prevTime = currentTime;
}

void Timer::Start() {

}

void Timer::Stop()
{
}

void Timer::Pause()
{
}

void Timer::Reset() {
	QueryPerformanceCounter((LARGE_INTEGER*) &currentTime);
	prevTime = currentTime;
}

float Timer::GetDeltaTime() const {
	return deltaTime;
}

