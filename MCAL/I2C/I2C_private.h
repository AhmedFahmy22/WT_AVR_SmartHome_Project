/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : I2C                                                 */
/* Version : 1.0                                                 */
/* Date    : 27 Dec 2023                                         */
/*****************************************************************/
#ifndef I2C_PRIVATE_H_
#define I2C_PRIVATE_H_

/*Registers*/
#define TWBR (*((volatile uint8*) 0x20))

#define TWCR (*((volatile uint8*) 0x56))

#define TWCR_BIT_TWINT 7
#define TWCR_BIT_TWEA  6
#define TWCR_BIT_TWSTA 5
#define TWCR_BIT_TWSTO 4
#define TWCR_BIT_TWWC  3
#define TWCR_BIT_TWEN  2
#define TWCR_BIT_TWIE  0


#define TWSR (*((volatile uint8*) 0x21))

#define TWSR_BIT_TWS3  3
#define TWSR_BIT_TWPS0 0

#define TWSR_TWS37_MASK  0b00000111
#define TWSR_TWPS01_MASK 0b11111100


#define TWDR (*((volatile uint8*) 0x23))

#define TWDR_BIT_TWDR0 0

#define TWAR (*((volatile uint8*) 0x22))

#define TWAR_BIT_TWA0  1
#define TWAR_BIT_TWGCE 0

#define TWAR_TWA06_MASK 0b00000001


#define I2C_START_CONDITION 0x08

#define I2C_REPEATED_START_CONDITION 0x10

#define I2C_SLA_WITH_W_AK 0x18

#define I2C_SLA_WITH_R_AK 0x40

#define I2C_MASTER_SEND_DATA_AK 0x28

#define I2C_MASTER_RECEIVE_DATA_NAK 0x58

/*Configuration Macros*/

#define I2C_ACKNOWLEDGE_BIT_DISABLE 0
#define I2C_ACKNOWLEDGE_BIT_ENABLE  1

#define I2C_STATE_DISABLE 0
#define I2C_STATE_ENABLE  1

#define I2C_INTERRUPT_DISABLE 0
#define I2C_INTERRUPT_ENABLE  1

#define I2C_BITRATE_PRESCALER_1  0
#define I2C_BITRATE_PRESCALER_4  1
#define I2C_BITRATE_PRESCALER_16 2
#define I2C_BITRATE_PRESCALER_64 3

#define I2C_GENERAL_CALL_DISABLE 0
#define I2C_GENERAL_CALL_ENABLE  1


/*I2C Bus Status*/
#define I2C_START_CONDITION 0x08

#define I2C_REPEATED_START_CONDITION 0x10

#define I2C_SLA_WITH_W_AK 0x18

#define I2C_SLA_WITH_R_AK 0x40

#define I2C_MASTER_SEND_DATA_AK 0x28

#define I2C_MASTER_RECEIVE_DATA_NAK 0x58

#endif
