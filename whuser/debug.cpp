#include "Global.h"

#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <stdarg.h>

void DEBUG_PRINT(char* fmt, ...)
{
	char str[1024] ;
    va_list	ap;

    va_start(ap, fmt);
    vsprintf_s(str, sizeof(str), fmt, ap);
    _cprintf("%s", str);

    va_end(ap);
}