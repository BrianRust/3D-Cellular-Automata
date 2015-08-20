//-------------------------------------------------------------
#include "Time.hpp"
#include <Windows.h>


//-------------------------------------------------------------

const double FRAME_TIME_SECONDS = 1.0 / 60.0;
double g_secondsPerCount = 0.0;

//-------------------------------------------------------------
double Time::GetCurrentTimeSeconds()
{
	if (g_secondsPerCount == 0.0) {

		LARGE_INTEGER countsPerSecond;
		QueryPerformanceFrequency( &countsPerSecond );

		g_secondsPerCount = 1.0 / countsPerSecond.QuadPart;

	}  // end if

	// Get the total clock cycles since the CPU has been awake
	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter( &performanceCount );

	// Return the current seconds amount
	return static_cast<double>( performanceCount.QuadPart ) * g_secondsPerCount;

} // end getCurrentTimeSeconds

