//
//	Filename	:	whuser.h
//	Date		:	2008 Jul 26
//	Description :
//
//	Copyright (c) 2008 David B. Conrad
//

#ifndef WHUSER_EXPORTS
	#define WHUSER_EXPORTS
#endif

#define DLL_NAME		"Longhorn Hold 'Em Poker Bot"
#define DLL_VERSION		0x00010001		// Version 0.1.1

#include "whuser.h"
#include "OpenHoldem.h"
#include "Global.h"
#include "PokerEngine.h"
#include "Debug.h"

#include <windows.h>
#include <atlstr.h>
#include <conio.h>
#include <iostream>
#include <fstream>

////////////////////////////////////
//  The one and only PokerEngine 
PokerEngine* theEngine;

double process_query(const char* pquery)
{
	Debug::log(LTRACE) << "::process_state( holdem_state* pstate )" << std::endl;
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
	Debug::log(LTRACE) << "::process_state( holdem_state* pstate )" << std::endl;

	// Update our PokerEngine with the latest table context
	theEngine->UpdateTableContext(m_pget_winholdem_symbol, pstate);

	return 0;
}

/*-------------------------------------------------------------------
	process_message()
	OPENHOLDEM RUNTIME ENTRY POINT
*/

WHUSER_API double process_message (const char* pmessage, const void* param)
{
	Debug::log(LTRACE) << "::process_message (const char* pmessage, const void* param)" << std::endl;

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
		Debug::log(LDEBUG4) << "query: " << (LPCSTR) param << std::endl;
		return process_query( (const char*)param ); 
	}

	// (PMESSAGE) "pfgws"
	// Called when DLL is first loaded to pass us a pointer to the OpenHoldem
	// get_winholdem_symbol() function.  We call this function to query
	// OpenHoldem symbols.
	if (strcmp(pmessage,"pfgws")==0) {	
		Debug::log(LDEBUG) << "MESSAGE: pfgws" << std::endl;
		m_pget_winholdem_symbol = (pfgws_t)param;
		return 0;
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "load")==0) { 
		Debug::log(LDEBUG) << "MESSAGE: event-load" << std::endl;
	}

	if (strcmp(pmessage,"event")==0 && strcmp((const char *) param, "unload")==0) { 
		Debug::log(LDEBUG) << "MESSAGE: event-unload" << std::endl;
	}

	return 0;
}


/*-------------------------------------------------------------------
	DllMain()
	This is the WinAPI entrypoint for DLL load/unload
*/

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	Debug::log(LTRACE) << "::DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)" << std::endl;

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			// Here we prepare the debug logger.  We'll be able to
			// build this out to be a more robust logger which can
			// log events to a file so that we can analyze the play

			Debug::InitializeDebugConsole();		// Prepare the output console
			Debug::SetLevel(LDEBUG);				// Define our reporting level (anything above the level is not output)

			Debug::log(LINFO) << DLL_NAME << std::endl;
			Debug::log(LINFO) << "Version: " << DLL_VERSION << std::endl;
			Debug::log(LINFO) << "Beginning new DLL instance" << std::endl; 
			Debug::LogDateAndTime(LINFO); Debug::log(LINFO) << std::endl;
			Debug::log(LINFO) << "------------------------------------------------------" << std::endl;

			// Initialize our one and only PokerEngine (NOTE: make this a singleton)
			theEngine = new PokerEngine();
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			Debug::DestroyDebugConsole();			// Return the output console to its previous state
			if (theEngine) { delete theEngine; theEngine = NULL; }
			break;
    }
    return TRUE;
}
