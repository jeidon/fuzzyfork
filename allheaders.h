#ifndef _INCFILES_INCLUDED
#define _INCFILES_INCLUDED

#define F_CPU 8000000uL
#define BAUD 19200
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#include <util/setbaud.h>

#include <avr/io.h>
//#include <inavr.h>
#include <ctype.h>
//#include "main.h"
//#include "spi.h"
#include "USART2.h"

#endif
