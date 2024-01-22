/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : USART                                               */
/* Version : 1.0                                                 */
/* Date    : 25 Dec 2023                                         */
/*****************************************************************/
#ifndef USART_PRIVATE_H_
#define USART_PRIVATE_H_

/*Registers*/
#define UCSRA (*((volatile uint8*) 0x2B))

#define UCSRA_BIT_RXC  7
#define UCSRA_BIT_TXC  6
#define UCSRA_BIT_UDRE 5
#define UCSRA_BIT_FE   4
#define UCSRA_BIT_DOR  3
#define UCSRA_BIT_PE   2
#define UCSRA_BIT_U2X  1
#define UCSRA_BIT_MPCM 0


#define UCSRB (*((volatile uint8*) 0x2A))

#define UCSRB_BIT_RXCIE 7
#define UCSRB_BIT_TXCIE 6
#define UCSRB_BIT_UDRIE 5
#define UCSRB_BIT_RXEN  4
#define UCSRB_BIT_TXEN  3
#define UCSRB_BIT_UCSZ2 2
#define UCSRB_BIT_RXB8  1
#define UCSRB_BIT_TXB8  0


#define UCSRC (*((volatile uint8*) 0x40))

#define UCSRC_BIT_URSEL 7
#define UCSRC_BIT_UMSEL 6
#define UCSRC_BIT_UPM1  5
#define UCSRC_BIT_UPM0  4
#define UCSRC_BIT_USBS  3
#define UCSRC_BIT_UCSZ1 2
#define UCSRC_BIT_UCSZ0 1
#define UCSRC_BIT_UCPOL 0

#define UCSRC_UCSZ01_MASK 0b11111001
#define UCSRC_UPM01_MASK  0b11001111

#define UDR   (*((volatile uint8*) 0x2C))
#define UBRRL (*((volatile uint8*) 0x29))


#define UBRRH (*((volatile uint8*) 0x40))
#define UBRRH_BIT_URSEL 7

/*Configuration Macros*/
#define USART_U2X_DISABLE 0
#define USART_U2X_ENABLE  1

#define USART_MULTI_PROCESSOR_DISABLE 0
#define USART_MULTI_PROCESSOR_ENABLE 1

#define USART_RX_INTERRUPT_DISABLE 0
#define USART_RX_INTERRUPT_ENABLE  1

#define USART_TX_INTERRUPT_DISABLE 0
#define USART_TX_INTERRUPT_ENABLE  1

#define USART_DATA_EMPTY_INTERRUPT_DISABLE 0
#define USART_DATA_EMPTY_INTERRUPT_ENABLE  1

#define USART_RECEIVER_DISABLE 0
#define USART_RECEIVER_ENABLE  1

#define USART_TRANSMITTER_DISABLE 0
#define USART_TRANSMITTER_ENABLE  1

#define USART_CHARACTER_SIZE_5BIT 0
#define USART_CHARACTER_SIZE_6BIT 1
#define USART_CHARACTER_SIZE_7BIT 2
#define USART_CHARACTER_SIZE_8BIT 3
#define USART_CHARACTER_SIZE_9BIT 7

#define USART_MODE_ASYNCHRONOUS 0
#define USART_MODE_SYNCHRONOUS  1

#define USART_PARITY_DISABLED 0
#define USART_PARITY_EVEN     2
#define USART_PARITY_ODD      3

#define USART_STOP_BIT_1BIT 0
#define USART_STOP_BIT_2BIT 1

#define USART_CLK_POLARITY_RISING_TRANSMIT  0
#define USART_CLK_POLARITY_FALLING_TRANSMIT 1

#define USART_BAUD_RATE_2400    207
#define USART_BAUD_RATE_4800    103
#define USART_BAUD_RATE_9600    51
#define USART_BAUD_RATE_14400   34
#define USART_BAUD_RATE_19200   25
#define USART_BAUD_RATE_28800   16
#define USART_BAUD_RATE_38400   12
#define USART_BAUD_RATE_57600   8
#define USART_BAUD_RATE_76800   6
#define USART_BAUD_RATE_115200  3
#define USART_BAUD_RATE_230400  1
#define USART_BAUD_RATE_250000  1
#define USART_BAUD_RATE_500000  0

#define USART_DATA_MAX_VALUE_9BIT 0x1FF

#endif
