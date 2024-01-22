/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : DIO                                                 */
/* Version : 1.0                                                 */
/* Date    : 09 Dec 2023                                         */
/*****************************************************************/
#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_

#define PORTA (*((volatile uint8*)0x3B))
#define DDRA  (*((volatile uint8*)0x3A))
#define PINA  (*((volatile uint8*)0x39))

#define PORTB (*((volatile uint8*)0x38))
#define DDRB  (*((volatile uint8*)0x37))
#define PINB  (*((volatile uint8*)0x36))

#define PORTC (*((volatile uint8*)0x35))
#define DDRC  (*((volatile uint8*)0x34))
#define PINC  (*((volatile uint8*)0x33))

#define PORTD (*((volatile uint8*)0x32))
#define DDRD  (*((volatile uint8*)0x31))
#define PIND  (*((volatile uint8*)0x30))

#define CONC(b7,b6,b5,b4,b3,b2,b1,b0) CONC_HELP(b7,b6,b5,b4,b3,b2,b1,b0)
#define CONC_HELP(b7,b6,b5,b4,b3,b2,b1,b0) 0b##b7##b6##b5##b4##b3##b2##b1##b0

#endif
