/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : EXTEEPROM                                           */
/* Version : 1.0                                                 */
/* Date    : 28 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"

#include "../../MCAL/I2C/I2C_interface.h"
#include "../../MCAL/TIMER0/TIMER0_interface.h"

#include "EXTEEPROM_interface.h"
#include "EXTEEPROM_private.h"
#include "EXTEEPROM_config.h"

void EXTEEPROM_voidInit(void)
{
    I2C_voidMasterInit();
}

tenuErrorI2C EXTEEPROM_enuRead(const uint8 u8MemoryLocationCpy, uint8* const pu8DataCpy)
{
    tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;

	/*Start Condition*/
    enuErrorStatLoc|=I2C_enuStartCondition();
    /*SLA with write*/
    enuErrorStatLoc|=I2C_enuSetSlaveAddressWithWrite(EXTEEPROM_SLAVE_ADDRESS);
    /*Send Memory Location*/
    enuErrorStatLoc|=I2C_enuMasterWriteData(u8MemoryLocationCpy);
    /*Repeat Start*/
    enuErrorStatLoc|=I2C_enuRepeatStart();
    /*SLA with Read*/
    enuErrorStatLoc|=I2C_enuSetSlaveAddressWithRead(EXTEEPROM_SLAVE_ADDRESS);
    /*Read Data*/
    enuErrorStatLoc|=I2C_enuMasterReadData(pu8DataCpy);
    /*Stop Condition*/
    I2C_voidStopCondition();

    return enuErrorStatLoc;
}

tenuErrorI2C EXTEEPROM_enuWrite(const uint8 u8MemoryLocationCpy, const uint8 u8DataCpy)
{
	tenuErrorI2C enuErrorStatLoc = E_NO_ERROR;
    /*Start Condition*/
	enuErrorStatLoc|=I2C_enuStartCondition();
    /*SLA with write*/
	enuErrorStatLoc|=I2C_enuSetSlaveAddressWithWrite(EXTEEPROM_SLAVE_ADDRESS);
    /*Send Memory Location*/
	enuErrorStatLoc|=I2C_enuMasterWriteData(u8MemoryLocationCpy);
    /*Send Data*/
	enuErrorStatLoc|=I2C_enuMasterWriteData(u8DataCpy);
    /*Stop Condition*/
    I2C_voidStopCondition();
    TIMER0_voidDelay(3);
    return enuErrorStatLoc;
}
