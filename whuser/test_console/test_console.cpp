// test_console.cpp : Defines the entry point for the console application.

/*-----------------------------------------------------------------
// NOTE: This is just a testing ground for code using the console.
// If you want to test new code: create a new function and
// call that function from main().  That way we can save
// code which is in research phase and not step on toes.
*/

#include "stdafx.h"
#include "researchcode-david.h"

#include <ctime>
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include <atlstr.h>

using namespace std;

#define WHUSER_API __declspec(dllimport)
typedef double (*process_message_t)(const char* message, const void* param);
process_message_t process_message;

/*------------------------------------------------------------------------------
	main()
*/

double GetSymbolFromDll(int chair, const char* name, bool& iserr)
{
	if (strcmp(name, "userchair") == 0)
		return 4;

	if (strcmp(name, "betround") == 0)
		return PREFLOP;
	return 0.0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE		hMod_dll;
	DWORD		err;
	CString		path;
	holdem_state pstate;

 	path =  "..\\Debug\\whuser.dll";
	hMod_dll = LoadLibrary((LPCWSTR)path);
	err = GetLastError();
    
	// If DLL is not loaded, pop an error
    if (hMod_dll==NULL)
    {
        cout << "Unable to load DLL from:" << path << "error=" << err << endl;;
        return 0;
	}

    process_message = (process_message_t) ::GetProcAddress(hMod_dll, (LPCSTR) 1);
	(process_message) ("pfgws", GetSymbolFromDll);

	pstate.m_cards[0] = CARD_NOCARD;
	pstate.m_cards[1] = CARD_NOCARD;
	pstate.m_cards[2] = CARD_NOCARD;
	pstate.m_cards[3] = CARD_NOCARD;
	pstate.m_cards[4] = CARD_NOCARD;
	pstate.m_player[4].m_cards[0] = MAKECARD(11, CLUBS);
	pstate.m_player[4].m_cards[1] = MAKECARD(11, HEARTS);
	(process_message) ("state", &pstate);

    if (FreeLibrary(hMod_dll))
        hMod_dll = NULL;

	return 1;
}