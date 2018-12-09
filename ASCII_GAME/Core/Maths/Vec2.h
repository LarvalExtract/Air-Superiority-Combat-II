#pragma once

template<class T>
class Vec2
{
public:
	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}

	T x;
	T y;
};