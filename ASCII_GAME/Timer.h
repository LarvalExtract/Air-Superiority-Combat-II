#pragma once

#include <chrono>

// Timer used to measure elapsed time
template<class T>
class Timer
{
	using clock = std::chrono::high_resolution_clock;
	using interval = std::chrono::duration<T, std::ratio<1>>;

public:
	Timer() : start(std::chrono::high_resolution_clock::now()) {};

	void Reset() { start = clock::now(); }
	T Elapsed() const { return std::chrono::duration_cast<interval>(clock::now() - start).count(); };

private:
	std::chrono::time_point<clock> start;
};