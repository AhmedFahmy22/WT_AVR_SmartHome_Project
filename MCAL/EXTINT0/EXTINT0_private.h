/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT0                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTINT0_PRIVATE_H_
#define EXTINT0_PRIVATE_H_

#define MCUCR  (*((volatile uint8*) 0x55))
#define GIFR   (*((volatile uint8*) 0x5A))
#define GICR   (*((volatile uint8*) 0x5B))

#define MCUCR_BIT_ISC01 1
#define MCUCR_BIT_ISC00 0

#define INTF0 1

#define INT0 6

#endif
