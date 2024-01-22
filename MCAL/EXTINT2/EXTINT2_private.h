/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : EXTINT2                                             */
/* Version : 1.0                                                 */
/* Date    : 17 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTINT2_PRIVATE_H_
#define EXTINT2_PRIVATE_H_

#define GIFR   (*((volatile uint8*) 0x5A))
#define GICR   (*((volatile uint8*) 0x5B))
#define MCUCSR (*((volatile uint8*) 0x54))

#define INT2 5

#define MCUCSR_BIT_ISC2 6

#endif
