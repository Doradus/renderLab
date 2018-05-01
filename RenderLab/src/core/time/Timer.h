#pragma once
#include <Windows.h>
class Timer {
public:
	Timer();
	~Timer();

	void		Tick();
	void		Start();
	void		Stop();
	void		Pause();
	void		Reset();
	float		GetDeltaTime() const;

private:
	__int64 currentTime;
	__int64 prevTime;

	bool	isStopped;
	bool	isPaused;
	float	deltaTime;
	double	secondsPerTick;
};