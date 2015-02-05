#include <stdio.h>
#include <SDL.h>

#include "ae.h"
#include "log.h"

/**
	Log to stdout with timestamp (milliseconds since SDL init).

	\note			Timestamp will be 0 if SDL not initialized
	\note			Not thread safe
	\param level	Log level
	\param msg		Log message
*/
void Log::log(int level, const char *msg)
{
#ifdef _DEBUG
	const char* msg_str[] = { "INFO", "WARNING", "ERROR" };

	unsigned ms = SDL_GetTicks();
	printf("%08d %-10s %s\n", ms, msg_str[level], msg);
#endif
}