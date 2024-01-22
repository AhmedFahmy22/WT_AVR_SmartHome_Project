/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT1                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTINT1_PRIVATE_H_
#define EXTINT1_PRIVATE_H_

#define MCUCR  (*((volatile uint8*) 0x55))
#define GIFR   (*((volatile uint8*) 0x5A))
#define GICR   (*((volatile uint8*) 0x5B))

#define MCUCR_BIT_ISC11 3
#define MCUCR_BIT_ISC10 2

#define INT1 7

#endif
