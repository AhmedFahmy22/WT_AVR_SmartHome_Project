/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : CLCD                                                */
/* Version : 1.0                                                 */
/* Date    : 11 Dec 2023                                         */
/*****************************************************************/
#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_

#include "../../LIB/STD_Types.h"

/*Macros*/
#define CLCD_COMM_CLEAR           0x01          /* replace all characters with ASCII 'space'                       */
#define CLCD_COMM_HOME            0x02          /* return cursor to first position on first line                   */
#define CLCD_COMM_CURSOR_OFF	  0x0C
#define CLCD_COMM_CURSOR_ON	      0x0E

/*Function Declarations*/
tenuErrorStatus CLCD_enuInit(void);
tenuErrorStatus CLCD_enuSendCommand(const uint8 u8CmdCpy);
tenuErrorStatus CLCD_enuWriteChar(const uint8 u8DataCpy);
tenuErrorStatus CLCD_enuWriteString (const uint8* const pu8StringCpy);
tenuErrorStatus CLCD_enuGotoxy (const uint8 u8LineNumCpy, const uint8 u8ColumnCpy);
tenuErrorStatus CLCD_enuWriteNum(const sint32 s32DataCpy);

#endif
