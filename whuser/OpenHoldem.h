#pragma once

#ifdef WHUSER_EXPORTS
#define WHUSER_API __declspec(dllexport)
#else
#define WHUSER_API __declspec(dllimport)
#endif

typedef double (*process_message_t)(const char* message, const void* param );
WHUSER_API double process_message( const char* message, const void* param );

////////////////////////////////////
//  extern to OpenHoldem API
pfgws_t m_pget_winholdem_symbol;

