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

#include "whuser.h"
#include "Global.h"
#include "PokerEngine.h"

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

double gws( int chair, const char* name, bool& iserr ) {
   return (*m_pget_winholdem_symbol)(chair,name,iserr);
}

double gws( const char* name ) {
   bool iserr;
   int mychair = (int) gws(0,"userchair",iserr);
   return gws(mychair,name,iserr);
}

double process_query( const char* pquery ) {
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

double process_state( holdem_state* pstate ) {

	// Update the DLL with the latest table context
	if (pstate!=NULL) {	m_holdem_state[ (++m_ndx)&0xff ] = *pstate; }

	// Update our PokerEngine with the latest table context
	theEngine->updateTableContext(m_pget_winholdem_symbol, pstate);

	return 0;
}

/////////////////////////////////////////////////////
//WINHOLDEM RUNTIME ENTRY POINT
/////////////////////////////////////////////////////
WHUSER_API double process_message (const char* pmessage, const void* param) {
	if (pmessage==NULL) { return 0; }
	if (param==NULL) { return 0; }

	// (PMESSAGE) "state"
	// The poker table framework is calling us to update our table context
	// and run any calculations if necessary
	if (strcmp(pmessage,"state")==0) { 
		return process_state( (holdem_state*)param ); 
	}

	if (strcmp(pmessage,"query")==0) { 
		_cprintf("query: %s\n", (LPCSTR) param);
		return process_query( (const char*)param ); 
	}

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
/////////////////////////////////////////////////////

/////////////////////////////////
//DLLMAIN
/////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)	{
		case DLL_PROCESS_ATTACH:
			AllocConsole();
			theEngine = new PokerEngine();
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
/////////////////////////////////
