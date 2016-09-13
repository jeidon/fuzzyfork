/*****************************************************************************
*
* Atmel Corporation
*
* File              : USART2.c
* Compiler          : IAR EWAAVR 2.28a/3.10c
* Revision          : $Revision: 1.3 $
* Date              : $Date: 17. mars 2004 14:47:16 $
* Updated by        : $Author: ltwa $
*
* Support mail      : avr@atmel.com
*
* Supported devices : All devices with a SPI and USART module can be used.
*                     The example is written for the ATmega8
*
* AppNote           : AVR303 - SPI-UART Gateway
*
// Author           : Andy Gayne. avr@gayne.co.uk   www.gd-technik.com
// Description      : Routines for interrupt controlled USART
****************************************************************************/

/* Includes */
#include "allheaders.h"
#include <avr/interrupt.h>

/* Static Variables USART0 */
static unsigned char USART0_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART0_RxHead;
static volatile unsigned char USART0_RxTail;
static unsigned char USART0_TxBuf[USART_TX_BUFFER_SIZE];
static volatile unsigned char USART0_TxHead;
static volatile unsigned char USART0_TxTail;

/* Static Variables USART1 */
//static unsigned char USART1_RxBuf[USART_RX_BUFFER_SIZE];
//static volatile unsigned char USART1_RxHead;
//static volatile unsigned char USART1_RxTail;
//static unsigned char USART1_TxBuf[USART_TX_BUFFER_SIZE];
//static volatile unsigned char USART1_TxHead;
//static volatile unsigned char USART1_TxTail;

/* Initialize USART */
void USART0_Init( unsigned int baudrate )
{
	unsigned char x;

	/* Set the baud rate */
	
	UBRR0H = (unsigned char) (baudrate>>8);                  
	UBRR0L = (unsigned char) baudrate;
	
	/* Enable UART receiver and transmitter */
	UCSR0B = ( ( 1 << RXCIE0 ) | ( 1 << RXEN0 ) | ( 1 << TXEN0 ) ); 
	
	/* Set frame format: 8 data, no parity, 1 stop */
	//UCSR0C = (1<<URSEL)|(0<<USBS0)|(1<<UCSZ10)|(1<<UCSZ00);
	//Because URSEL = UMSELn1 & UMSELn0
	//UCSR0C = (1<<UMSEL01)|(1<<UMSEL00)|(0<<USBS0)|(1<<UCSZ10)|(1<<UCSZ00);
	//UCSR0C = (0<<USBS0)|(1<<UCSZ10)|(1<<UCSZ00);
	
	/* Flush receive buffer */
	x = 0; 			    

	USART0_RxTail = x;
	USART0_RxHead = x;
	USART0_TxTail = x;
	USART0_TxHead = x;
}

/* Interrupt handlers */
//#pragma vector=USART_RXC_vect 
//__interrupt void USART_RX_interrupt( void )
ISR(USART0_RX_vect)
{
	unsigned char data;
	unsigned char tmphead;

	/* Read the received data */
	data = UDR0;
	/* Calculate buffer index */
	tmphead = ( USART0_RxHead + 1 ) & USART_RX_BUFFER_MASK;
	USART0_RxHead = tmphead;      /* Store new index */

	if ( tmphead == USART0_RxTail )
	{
		/* ERROR! Receive buffer overflow */
	}
	
	USART0_RxBuf[tmphead] = data; /* Store received data in buffer */
}

//#pragma vector=USART_UDRE_vect 
//__interrupt void USART_TX_interrupt( void )
ISR(USART0_TX_vect)
{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if ( USART0_TxHead != USART0_TxTail )
	{
		/* Calculate buffer index */
		tmptail = ( USART0_TxTail + 1 ) & USART_TX_BUFFER_MASK;
		USART0_TxTail = tmptail;      /* Store new index */
	
		UDR0 = USART0_TxBuf[tmptail];  /* Start transmission */
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0);         /* Disable UDRE interrupt */
	}
}

/* Read and write functions */
char USART0_Receive( void )
{
	unsigned char tmptail;
	
	while ( USART0_RxHead == USART0_RxTail );  /* Wait for incoming data */

	tmptail = ( USART0_RxTail + 1 ) & USART_RX_BUFFER_MASK;/* Calculate buffer index */
	
	USART0_RxTail = tmptail;                /* Store new index */	

	return USART0_RxBuf[tmptail];  /* Return data */
}

void USART0_Transmit( unsigned char data )
{
	unsigned char tmphead;
	/* Calculate buffer index */
	tmphead = ( USART0_TxHead + 1 ) & USART_TX_BUFFER_MASK; /* Wait for free space in buffer */
	while ( tmphead == USART0_TxTail );

//	USART0_TxBuf[tmphead] = data;           /* Store data in buffer */
//	USART0_TxHead = tmphead;                /* Store new index */

//	UCSR0B |= (1<<UDRIE0);                    /* Enable UDRE interrupt */
}

//unsigned char DataInReceiveBuffer0( void )
//{
//	return ( USART0_RxHead != USART0_RxTail ); /* Return 0 (FALSE) if the receive buffer is empty */
//}
/* Initialize USART */
//void USART1_Init( unsigned int baudrate )
//{
	//unsigned char x;
//
	///* Set the baud rate */
	//
	//UBRR1H = (unsigned char) (baudrate>>8);
	//UBRR1L = (unsigned char) baudrate;
	//
	///* Enable UART receiver and transmitter */
	//UCSR1B = ( ( 1 << RXCIE1 ) | ( 1 << RXEN1 ) | ( 1 << TXEN1 ) );
	//
	///* Set frame format: 8 data, no parity, 1 stop */
	////UCSR0C = (1<<URSEL)|(0<<USBS0)|(1<<UCSZ10)|(1<<UCSZ00);
	////Because URSEL = UMSELn1 & UMSELn0
	////UCSR0C = (1<<UMSEL01)|(1<<UMSEL00)|(0<<USBS0)|(1<<UCSZ10)|(1<<UCSZ00);
	////UCSR0C = (0<<USBS0)|(1<<UCSZ10)|(1<<UCSZ00);
	//
	///* Flush receive buffer */
	//x = 0;
//
	//USART1_RxTail = x;
	//USART1_RxHead = x;
	//USART1_TxTail = x;
	//USART1_TxHead = x;
//}
//
///* Interrupt handlers */
////#pragma vector=USART_RXC_vect
////__interrupt void USART_RX_interrupt( void )
//ISR(USART1_RX_vect)
//{
	//unsigned char data;
	//unsigned char tmphead;
//
	///* Read the received data */
	//data = UDR1;
	///* Calculate buffer index */
	//tmphead = ( USART1_RxHead + 1 ) & USART_RX_BUFFER_MASK;
	//USART1_RxHead = tmphead;      /* Store new index */
//
	//if ( tmphead == USART1_RxTail )
	//{
		///* ERROR! Receive buffer overflow */
	//}
	//
	//USART1_RxBuf[tmphead] = data; /* Store received data in buffer */
//}
//
////#pragma vector=USART_UDRE_vect
////__interrupt void USART_TX_interrupt( void )
//ISR(USART1_TX_vect)
//{
	//unsigned char tmptail;
//
	///* Check if all data is transmitted */
	//if ( USART1_TxHead != USART1_TxTail )
	//{
		///* Calculate buffer index */
		//tmptail = ( USART1_TxTail + 1 ) & USART_TX_BUFFER_MASK;
		//USART1_TxTail = tmptail;      /* Store new index */
		//
		//UDR1 = USART1_TxBuf[tmptail];  /* Start transmission */
	//}
	//else
	//{
		//UCSR1B &= ~(1<<UDRIE1);         /* Disable UDRE interrupt */
	//}
//}
//
///* Read and write functions */
//unsigned char USART1_Receive( void )
//{
	//unsigned char tmptail;
	//
	//if ( USART1_RxHead != USART1_RxTail )  /* Wait for incoming data */
	//{
//
		//tmptail = ( USART1_RxTail + 1 ) & USART_RX_BUFFER_MASK;/* Calculate buffer index */
	//
		//USART1_RxTail = tmptail;                /* Store new index */
	//
		//return USART1_RxBuf[tmptail];  /* Return data */
	//}
//
	//return 0;
//}
//
//void USART1_Transmit( unsigned char data )
//{
	//unsigned char tmphead;
	///* Calculate buffer index */
	//tmphead = ( USART1_TxHead + 1 ) & USART_TX_BUFFER_MASK; /* Wait for free space in buffer */
	//while ( tmphead == USART1_TxTail );
//
	//USART1_TxBuf[tmphead] = data;           /* Store data in buffer */
	//USART1_TxHead = tmphead;                /* Store new index */
//
	//UCSR1B |= (1<<UDRIE1);                    /* Enable UDRE interrupt */
//}
////unsigned char DataInReceiveBuffer1( void )
////{
////return ( USART1_RxHead != USART1_RxTail ); /* Return 0 (FALSE) if the receive buffer is empty */
////}
//
