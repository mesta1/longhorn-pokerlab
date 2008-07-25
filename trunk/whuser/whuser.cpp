/*
	Filename	:	whuser.cpp
	Author(s)	:	winholdem development
	Date		:	2004-JUN-25
	Copyright	:	(c) 2004 HixoxiH Software
	History		:
*/

#ifndef WHUSER_EXPORTS
	#define WHUSER_EXPORTS
#endif

#define DLL_NAME		"Longhorn Limit Hold 'Em Poker Bot"
#define DLL_VERSION		0x00010001		// Version 0.1.1

#include "whuser.h"
#include "Global.h"
#include "PokerEngine.h"
#include "Debug.h"

#include <windows.h>
#include <atlstr.h>
#include <conio.h>

////////////////////////////////////
//consecutive states
holdem_state	m_holdem_state[256];
unsigned char	m_ndx;
////////////////////////////////////

PokerEngine* theEngine;
pfgws_t m_pget_winholdem_symbol;

double process_query(const char* pquery)
{
	Debug::log(Debug::TRACE) << "::process_state( holdem_state* pstate )" << std::endl;
	if (pquery==NULL)
		return 0;

	if (strncmp(pquery,"dll$getpreflopaction",11)==0)
	{
		int action = theEngine->getPreflopAction();
//		_cprintf("preflop action: %d\n", action);
		return action;
	}

	return 0;
}

double process_state( holdem_state* pstate )
{
	Debug::log(Debug::TRACE) << "::process_state( holdem_state* pstate )" << std::endl;

	// Update the DLL with the latest table context
	if (pstate!=NULL) {	m_holdem_state[ (++m_ndx)&0xff ] = *pstate; }

	// Update our PokerEngine with the latest table context
	theEngine->updateTableContext(m_pget_winholdem_symbol, pstate);

	return 0;
}

/*-------------------------------------------------------------------
	process_message()
	OPENHOLDEM RUNTIME ENTRY POINT
*/

WHUSER_API double process_message (const char* pmessage, const void* param)
{
	Debug::log(Debug::TRACE) << "::process_message (const char* pmessage, const void* param)" << std::endl;

	if (pmessage==NULL) { return 0; }
	if (param==NULL) { return 0; }

	// (PMESSAGE) "state"
	// The OpenHoldem framework is calling us to update our table context
	// and run any calculations if necessary
	if (strcmp(pmessage,"state")==0) { 
		return process_state( (holdem_state*)param ); 
	}

	// (PMESSAGE) "query"
	// The OpenHoldem framework is calling us to ask us a question.
	if (strcmp(pmessage,"query")==0) { 
		_cprintf("query: %s\n", (LPCSTR) param);
		return process_query( (const char*)param ); 
	}

	// (PMESSAGE) "pfgws"
	// Called when DLL is first loaded to pass us a pointer to the OpenHoldem
	// get_winholdem_symbol() function.  We call this function to query
	// OpenHoldem symbols.
	if (strcmp(pmessage,"pfgws")==0) {	
		_cprintf("MESSAGE: pfgws\n");
		m_pget_winholdem_symbol = (pfgws_t)param;
		return 0;
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "load")==0) { 
		_cprintf("MESSAGE: event-load\n");
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "unload")==0) { 
		_cprintf("MESSAGE: event-unload\n");
	}

	return 0;
}


/*-------------------------------------------------------------------
	DllMain()
	This is the WinAPI entrypoint for DLL load/unload
*/

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	Debug::log(Debug::TRACE) << "::DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)" << std::endl;

	switch (ul_reason_for_call)	{
		case DLL_PROCESS_ATTACH:
			AllocConsole();
			Debug::log(Debug::INFO) << DLL_NAME << std::endl;
			Debug::log(Debug::INFO) << "Version:" << DLL_VERSION << std::endl;
			theEngine = new PokerEngine();		// Initialize our one and only PokerEngine
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			FreeConsole();
			if (theEngine) { delete theEngine; theEngine = NULL; }
			break;
    }
    return TRUE;
}
