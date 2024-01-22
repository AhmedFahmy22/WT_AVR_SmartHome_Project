/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : I2C                                                 */
/* Version : 1.0                                                 */
/* Date    : 27 Dec 2023                                         */
/*****************************************************************/
#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_

typedef enum {
    E_NO_ERROR,
    E_NULL_POINTER,
    E_START_CONDITION,
    E_REPEAT_START,
    E_SLA_W_NOTAK,
    E_MASTER_SEND_DATA,
    E_MASTER_RECEIVE_DATA

}tenuErrorI2C;

void I2C_voidMasterInit(void);

void I2C_voidSlaveInit(void);

tenuErrorI2C I2C_enuStartCondition(void);

tenuErrorI2C I2C_enuRepeatStart(void);

tenuErrorI2C I2C_enuSetSlaveAddressWithWrite(const uint8 u8SlaveAddressCpy);

tenuErrorI2C I2C_enuSetSlaveAddressWithRead(const uint8 u8SlaveAddressCpy);

tenuErrorI2C I2C_enuMasterWriteData(const uint8 u8DataCpy);

tenuErrorI2C I2C_enuMasterReadData(uint8* const pu8DataCpy);

void I2C_voidStopCondition(void);

void I2C_voidIntEnable(void);

void I2C_voidIntDisable(void);

tenuErrorStatus I2C_enuSetCallBack(const pf pfI2CISRCpy);

#endif
