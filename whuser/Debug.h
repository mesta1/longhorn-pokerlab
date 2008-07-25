#pragma once
#include <iostream>

class Debug
{
public:
	static std::ostream& log(int);

	enum LogLevel 
	{
		ERROR, 
		WARNING, 
		INFO, 
		DEBUG, 
		TRACE,
		DEBUG2, 
		DEBUG3, 
		DEBUG4
	};

protected:
	Debug(void);
	~Debug(void);

private:
};

