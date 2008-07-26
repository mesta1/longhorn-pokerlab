#include "Debug.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>

std::ofstream Debug::cnull;
std::ofstream Debug::out_stream;
std::ofstream Debug::error_stream;
std::ifstream Debug::in_stream;

std::streambuf* Debug::old_cout = 0;
std::streambuf* Debug::old_cerr = 0;
std::streambuf* Debug::old_cin = 0;

int Debug::reporting_level = LINFO;

Debug::Debug(void)
{
}

Debug::~Debug(void)
{
}


std::ostream& Debug::log(int level)
{
	time_t rawtime;
	struct tm *timeinfo;
	char	buf[64];
	std::ostream* os;

	// Determine what our reporting level and only return a valid
	// stream if the specified level is less than to or greater than
	// the reporting level
	if ((level) <= reporting_level)
	{
		time (&rawtime);
		timeinfo = localtime(&rawtime);

		sprintf(buf, "[%02d:%02d:%02d] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		os = &std::cout;
		(*os) << buf;

		return (*os);
	}
	else
		return cnull;		// kludge
}
void Debug::SetLevel(int level)
{
	reporting_level = level;
}

void Debug::InitializeDebugConsole()
{
	// create a console window
	AllocConsole();

	// create a NULL file pointer to dump a stream when
	// it is outside our reporting level
	//m_NULLSTREAM.close();

	// redirect std::cout to our console window
	old_cout = std::cout.rdbuf();
	out_stream.open("CONOUT$");
	std::cout.rdbuf(out_stream.rdbuf());

	// redirect std::cerr to our console window
	old_cerr = std::cerr.rdbuf();
	error_stream.open("CONOUT$");
	std::cerr.rdbuf(error_stream.rdbuf());

	// redirect std::cin to our console window
	old_cin = std::cin.rdbuf();
	in_stream.open("CONIN$");
	std::cin.rdbuf(in_stream.rdbuf());
}

void Debug::DestroyDebugConsole()
{
	// reset the standard streams
	std::cin.rdbuf(old_cin);
	std::cerr.rdbuf(old_cerr);
	std::cout.rdbuf(old_cout);

	// remove the console window
	FreeConsole();

}

void Debug::LogDateAndTime(int level)
{
	time_t rawtime;
	struct tm *timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	log(level) << "The current date/time is: " << asctime(timeinfo);

}


