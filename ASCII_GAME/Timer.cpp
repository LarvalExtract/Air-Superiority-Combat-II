#include "Timer.h"

Timer::Timer() :
	start(std::chrono::high_resolution_clock::now())
{
}

void Timer::Reset()
{
	start = std::chrono::high_resolution_clock::now();
}

double Timer::Elapsed() const
{
	return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(std::chrono::high_resolution_clock::now() - start).count();
}