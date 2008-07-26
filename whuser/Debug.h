#pragma once

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include <iostream>
#include <fstream>

#define		LOG(a)		Debug::log(a)

enum ReportingLevel 
{
	LERROR, 
	LWARNING, 
	LINFO, 
	LDEBUG, 
	LTRACE,
	LDEBUG2, 
	LDEBUG3, 
	LDEBUG4
};

class Debug
{
public:
	static std::ostream& log(int);
	static void InitializeDebugConsole(void);
	static void DestroyDebugConsole(void);
	static void	SetLevel(int);
	static void LogDateAndTime(int);

protected:
	Debug(void);
	~Debug(void);

private:
	static std::ofstream cnull;
	static std::ofstream out_stream;
	static std::ofstream error_stream;
	static std::ifstream in_stream;

	static std::streambuf* old_cout;
	static std::streambuf* old_cerr;
	static std::streambuf* old_cin;

	static int reporting_level;

};

#endif