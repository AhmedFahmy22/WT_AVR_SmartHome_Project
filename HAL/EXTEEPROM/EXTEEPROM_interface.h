/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : EXTEEPROM                                           */
/* Version : 1.0                                                 */
/* Date    : 28 Dec 2023                                         */
/*****************************************************************/
#ifndef EXTEEPROM_INTERFACE_H_
#define EXTEEPROM_INTERFACE_H_

void EXTEEPROM_voidInit(void);

tenuErrorI2C EXTEEPROM_enuRead(const uint8 u8MemoryLocationCpy, uint8* const pu8DataCpy);
tenuErrorI2C EXTEEPROM_enuWrite(const uint8 u8MemoryLocationCpy, const uint8 u8DataCpy);


#endif
