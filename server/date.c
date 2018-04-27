#include <stdio.h>
#include <time.h>
#include "date.h"

char* returnDate(char * buffer)
{
	time_t rawtime;
	struct tm *timeinfo;

	time (&rawtime);

	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%d-%m-%Y---%I:%M:%S%p", timeinfo);

	return buffer;
}