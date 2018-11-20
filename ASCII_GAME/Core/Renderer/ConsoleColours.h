#pragma once

// colours from wincon.h
//#define FOREGROUND_BLUE      0x0001 // text color contains blue.
//#define FOREGROUND_GREEN     0x0002 // text color contains green.
//#define FOREGROUND_RED       0x0004 // text color contains red.
//#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
//
//#define BACKGROUND_BLUE      0x0010 // background color contains blue.
//#define BACKGROUND_GREEN     0x0020 // background color contains green.
//#define BACKGROUND_RED       0x0040 // background color contains red.
//#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.

enum ConsoleColour : unsigned short
{
	Black = 0,

	ForegroundRed			= 0x0004 | 0x0008,
	ForegroundGreen			= 0x0002 | 0x0008,
	ForegroundBlue			= 0x0001 | 0x0008,
	ForegroundYellow		= 0x0004 | 0x0002 | 0x0008,
	ForegroundPink			= 0x0004 | 0x0001 | 0x0008,
	ForegroundCyan			= 0x0001 | 0x0002 | 0x0008,
	ForegroundWhite			= 0x0004 | 0x0001 | 0x0002 | 0x0008,
	ForegroundGrey			= 0x0004 | 0x0001 | 0x0002,
	ForegroundDarkRed		= 0x0004,
	ForegroundDarkGreen		= 0x0002,
	ForegroundDarkBlue		= 0x0001,
	ForegroundDarkYellow	= 0x0004 | 0x0002,
	ForegroundDarkPink		= 0x0004 | 0x0001,
	ForegroundDarkCyan		= 0x0001 | 0x0002,
	ForegroundDarkGrey		= 0x0008,

	BackgroundRed			= 0x0040 | 0x0080,
	BackgroundGreen			= 0x0020 | 0x0080,
	BackgroundBlue			= 0x0010 | 0x0080,
	BackgroundYellow		= 0x0040 | 0x0020 | 0x0080,
	BackgroundPink			= 0x0040 | 0x0010 | 0x0080,
	BackgroundCyan			= 0x0010 | 0x0020 | 0x0080,
	BackgroundWhite			= 0x0040 | 0x0010 | 0x0020 | 0x0080,
	BackgroundGrey			= 0x0040 | 0x0010 | 0x0020,
	BackgroundDarkRed		= 0x0040,
	BackgroundDarkGreen		= 0x0020,
	BackgroundDarkBlue		= 0x0010,
	BackgroundDarkYellow	= 0x0040 | 0x0020,
	BackgroundDarkPink		= 0x0040 | 0x0010,
	BackgroundDarkCyan		= 0x0010 | 0x0020,
	BackgroundDarkGrey		= 0x0080
};

inline ConsoleColour operator|(ConsoleColour lhs, const ConsoleColour& rhs)
{
	return lhs | rhs;
}