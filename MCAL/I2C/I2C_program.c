/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : I2C                                                 */
/* Version : 1.0                                                 */
/* Date    : 27 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "I2C_interface.h"
#include "I2C_private.h"
#include "I2C_config.h"

/*Global Variables*/
pf pfI2CISRGlb = NULL_PTR;

void I2C_voidMasterInit(void)
{
    /*Set Clock Config*/
    TWBR = I2C_DIVISION_FACTOR;
    TWSR = (TWSR & TWSR_TWPS01_MASK) | I2C_BITRATE_PRESCALER;
    /*Disable ACK bit*/
    CLR_BIT(TWCR, TWCR_BIT_TWEA);
    /*General Call*/
    #if I2C_GENERAL_CALL == I2C_GENERAL_CALL_ENABLE
    SET_BIT(TWAR, TWAR_BIT_TWGCE);
    #elif I2C_GENERAL_CALL == I2C_GENERAL_CALL_DISABLE
    CLR_BIT(TWAR, TWAR_BIT_TWGCE);
    #endif
    /*Set master address*/
    TWAR = (TWAR & TWAR_TWA06_MASK) | (I2C_MASTER_ADDRESS<<TWAR_BIT_TWA0);
    /*TWI State*/
    #if I2C_STATE == I2C_STATE_ENABLE
    SET_BIT(TWCR, TWCR_BIT_TWEN);
    #elif I2C_STATE == I2C_STATE_DISABLE
    CLR_BIT(TWCR, TWCR_BIT_TWEN);
    #endif
    /*Interrupt*/
    #if I2C_INTERRUPT == I2C_INTERRUPT_DISABLE
    CLR_BIT(TWCR, TWCR_BIT_TWIE);
    #elif I2C_INTERRUPT == I2C_INTERRUPT_ENABLE
    SET_BIT(TWCR, TWCR_BIT_TWIE);
    #endif
}

void I2C_voidSlaveInit(void)
{
    /*Enable ACK bit*/
    SET_BIT(TWCR, TWCR_BIT_TWEA);
    /*General Call*/
    #if I2C_GENERAL_CALL == I2C_GENERAL_CALL_ENABLE
    SET_BIT(TWAR, TWAR_BIT_TWGCE);
    #elif I2C_GENERAL_CALL == I2C_GENERAL_CALL_DISABLE
    CLR_BIT(TWAR, TWAR_BIT_TWGCE);
    #endif
    /*Set slave address*/
    TWAR = (TWAR & TWAR_TWA06_MASK) | (I2C_SLAVE_ADDRESS<<TWAR_BIT_TWA0);
    /*TWI State*/
    #if I2C_STATE == I2C_STATE_ENABLE
    SET_BIT(TWCR, TWCR_BIT_TWEN);
    #elif I2C_STATE == I2C_STATE_DISABLE
    CLR_BIT(TWCR, TWCR_BIT_TWEN);
    #endif
    /*Interrupt*/
    #if I2C_INTERRUPT == I2C_INTERRUPT_DISABLE
    CLR_BIT(TWCR, TWCR_BIT_TWIE);
    #elif I2C_INTERRUPT == I2C_INTERRUPT_ENABLE
    SET_BIT(TWCR, TWCR_BIT_TWIE);
    #endif
}

tenuErrorI2C I2C_enuStartCondition(void)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;

    /*Enable start condition*/
    SET_BIT(TWCR, TWCR_BIT_TWSTA);
    /*Clear interrupt flag to start the operation*/
    SET_BIT(TWCR, TWCR_BIT_TWINT);
    /*Enable TWI*/
    SET_BIT(TWCR, TWCR_BIT_TWEN);

    /*Busy wait on interrupt flag*/
    while(GET_BIT(TWCR, TWCR_BIT_TWINT)==0);

    if((TWSR & 0xF8) != I2C_START_CONDITION)
    {
        enuErrorStatLoc = E_START_CONDITION;
    }
    else
    {
        /* Do Nothing */
    }

    return enuErrorStatLoc;
}

tenuErrorI2C I2C_enuRepeatStart(void)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;

    /*Enable start condition*/
    SET_BIT(TWCR, TWCR_BIT_TWSTA);
    /*Clear interrupt flag to start the operation*/
    SET_BIT(TWCR, TWCR_BIT_TWINT);
    /*Enable TWI*/
    SET_BIT(TWCR, TWCR_BIT_TWEN);

    /*Busy wait on interrupt flag*/
    while(GET_BIT(TWCR, TWCR_BIT_TWINT)==0);

    if((TWSR & 0xF8) != I2C_REPEATED_START_CONDITION)
    {
        enuErrorStatLoc = E_REPEAT_START;
    }
    else
    {
        /* Do Nothing */
    }

    return enuErrorStatLoc;
}

tenuErrorI2C I2C_enuSetSlaveAddressWithWrite(const uint8 u8SlaveAddressCpy)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;

    /*Set Slave address*/
    TWDR = u8SlaveAddressCpy<<1;
    /*Clear bit0 (write)*/
    CLR_BIT(TWDR, TWDR_BIT_TWDR0);
    /*Clear the start condition bit*/
    CLR_BIT(TWCR, TWCR_BIT_TWSTA);
    /*Clear interrupt flag to start the operation*/
    SET_BIT(TWCR, TWCR_BIT_TWINT);
    /*Enable TWI*/
    SET_BIT(TWCR, TWCR_BIT_TWEN);

    /*Busy wait on interrupt flag*/
    while(GET_BIT(TWCR, TWCR_BIT_TWINT)==0);

    if((TWSR & 0xF8) != I2C_SLA_WITH_W_AK)
    {
        enuErrorStatLoc = E_SLA_W_NOTAK;
    }
    else
    {
        /* Do Nothing */
    }

    return enuErrorStatLoc;
}

tenuErrorI2C I2C_enuSetSlaveAddressWithRead(const uint8 u8SlaveAddressCpy)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;

    /*Set Slave address*/
    TWDR = u8SlaveAddressCpy<<1;
    /*Set bit0 (read)*/
    SET_BIT(TWDR, TWDR_BIT_TWDR0);
    /*Clear the start condition bit*/
    CLR_BIT(TWCR, TWCR_BIT_TWSTA);
    /*Clear interrupt flag to start the operation*/
    SET_BIT(TWCR, TWCR_BIT_TWINT);
    /*Enable TWI*/
    SET_BIT(TWCR, TWCR_BIT_TWEN);

    /*Busy wait on interrupt flag*/
    while(GET_BIT(TWCR, TWCR_BIT_TWINT)==0);

    if((TWSR & 0xF8) != I2C_SLA_WITH_R_AK)
    {
        enuErrorStatLoc = E_SLA_W_NOTAK;
    }
    else
    {
        /* Do Nothing */
    }

    return enuErrorStatLoc;
}

tenuErrorI2C I2C_enuMasterWriteData(const uint8 u8DataCpy)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;

    /*Write data in TWDR*/
    TWDR = u8DataCpy;
    /*Clear interrupt flag to start the operation*/
    SET_BIT(TWCR, TWCR_BIT_TWINT);
    /*Enable TWI*/
    SET_BIT(TWCR, TWCR_BIT_TWEN);

    /*Busy wait on interrupt flag*/
    while(GET_BIT(TWCR, TWCR_BIT_TWINT)==0);

    if((TWSR & 0xF8) != I2C_MASTER_SEND_DATA_AK)
    {
        enuErrorStatLoc = E_MASTER_SEND_DATA;
    }
    else
    {
        /* Do Nothing */
    }

    return enuErrorStatLoc;
}

tenuErrorI2C I2C_enuMasterReadData(uint8* const pu8DataCpy)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;
    if(pu8DataCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NULL_POINTER;
    }
    else
    {
        /* Enable TWI */
        SET_BIT(TWCR , TWCR_BIT_TWEN);
        /* Clear interrupt flag to start the operation */
        SET_BIT(TWCR , TWCR_BIT_TWINT);


        //TWCR = 0b10000100;
        /*Wait until the interrupt flag us raised again  */
        while(GET_BIT(TWCR , TWCR_BIT_TWINT) == 0);

        if((TWSR & 0xF8) != I2C_MASTER_RECEIVE_DATA_NAK)
        {
            enuErrorStatLoc = E_MASTER_RECEIVE_DATA;
        }
        else
        {
            *pu8DataCpy = TWDR;
        }
    }

    return enuErrorStatLoc;
}

void I2C_voidStopCondition(void)
{
    /*Set stop condition bit*/
    SET_BIT(TWCR, TWCR_BIT_TWSTO);
    /*Clear interrupt flag to start the operation*/
    SET_BIT(TWCR, TWCR_BIT_TWINT);
    /*Enable TWI*/
    SET_BIT(TWCR, TWCR_BIT_TWEN);
}

void I2C_voidIntEnable(void)
{
    SET_BIT(TWCR, TWCR_BIT_TWIE);
}

void I2C_voidIntDisable(void)
{
    CLR_BIT(TWCR, TWCR_BIT_TWIE);
}

tenuErrorStatus I2C_enuSetCallBack(const pf pfI2CISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfI2CISRCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        pfI2CISRGlb = pfI2CISRCpy;
    }

    return enuErrorStatLoc;
}

void __vector_19 (void)  __attribute__ ((signal,used));
void __vector_19 (void)
{
    if(pfI2CISRGlb!=NULL_PTR)
    {
        pfI2CISRGlb();
    }
    else
    {
        /*Nothing*/
    }
}
