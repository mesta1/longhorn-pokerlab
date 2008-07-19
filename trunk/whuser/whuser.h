/*
	Filename	:	whuser.h
	Author(s)	:	winholdem development
	Date		:	2004-JUN-25
	Copyright	:	(c) 2004 HixoxiH Software
	History		:
*/

#ifndef _whuser_h_
#define _whuser_h_

#ifdef WHUSER_EXPORTS
#define WHUSER_API __declspec(dllexport)
#else
#define WHUSER_API __declspec(dllimport)
#endif

#include "Global.h"

typedef double (*process_message_t)(const char* message, const void* param );
WHUSER_API double process_message( const char* message, const void* param );

double process_state(holdem_state* pstate);
double process_query(const char* pquery);

#endif
