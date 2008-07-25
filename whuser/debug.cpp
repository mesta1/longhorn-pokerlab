#include "Global.h"
#include "Debug.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>

Debug::Debug(void)
{
}

Debug::~Debug(void)
{
}


std::ostream& Debug::log(int LogLevel)
{
	time_t rawtime;
	struct tm *timeinfo;
	char	buf[64];
	std::ostream* os = &std::cout;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

//	sprintf(buf, "[%02d:%02d:%02d-%02d%02d%02d] ", timeinfo->tm_hour, timeinfo->tm_min,
//		timeinfo->tm_sec, (timeinfo->tm_year+1900), timeinfo->tm_mon, timeinfo->tm_mday);

	sprintf(buf, "[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	(*os) << buf;

	return (*os);
}
