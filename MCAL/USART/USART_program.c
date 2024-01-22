/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : USART                                               */
/* Version : 1.0                                                 */
/* Date    : 25 Dec 2023                                         */
/*****************************************************************/
#include "../../LIB/STD_Types.h"
#include "../../LIB/BIT_Math.h"

#include "USART_interface.h"
#include "USART_private.h"
#include "USART_config.h"

/*Global Variables*/
pf pfReceiveISRGlb = NULL_PTR;
pf pfTransmitISRGlb = NULL_PTR;


/*Function Definitions*/
void USART_voidInit(void)
{
    uint8 u8TempLoc = 0;
    /*****UCSRA*****/
    /*Double Speed Enable/Disable*/
    #if USART_U2X == USART_U2X_DISABLE
    CLR_BIT(UCSRA, UCSRA_BIT_U2X);
    #elif USART_U2X == USART_U2X_ENABLE
    SET_BIT(UCSRA, UCSRA_BIT_U2X);
    #endif

    /*Multi-processor ENABLE/Disable*/
    #if USART_MULTI_PROCESSOR == USART_MULTI_PROCESSOR_DISABLE
    CLR_BIT(UCSRA, UCSRA_BIT_MPCM);
    #elif USART_MULTI_PROCESSOR == USART_MULTI_PROCESSOR_ENABLE
    SET_BIT(UCSRA, UCSRA_BIT_MPCM);
    #endif

    /*****UCSRB*****/
    /*Character Size 3rd Bit*/
    CLR_BIT(UCSRB, UCSRB_BIT_UCSZ2);
    UCSRB |= (GET_BIT(USART_CHARACTER_SIZE,2)<<2);

    /*****UCSRC*****/
    /*UCSRC Select*/
    SET_BIT(u8TempLoc, UCSRC_BIT_URSEL);

    /*Mode Select Asynch./Synch.*/
    #if USART_MODE == USART_MODE_ASYNCHRONOUS
    CLR_BIT(u8TempLoc, UCSRC_BIT_UMSEL);
    #elif USART_MODE == USART_MODE_SYNCHRONOUS
    SET_BIT(u8TempLoc, UCSRC_BIT_UMSEL);
    #endif

    /*Parity*/
    u8TempLoc = (u8TempLoc & UCSRC_UPM01_MASK) | (USART_PARITY<<UCSRC_BIT_UPM0);

    /*Stop Bit 1-bit/2-bit*/
    #if USART_STOP_BIT == USART_STOP_BIT_1BIT
    CLR_BIT(u8TempLoc, UCSRC_BIT_USBS);
    #elif USART_STOP_BIT == USART_STOP_BIT_2BIT
    SET_BIT(u8TempLoc, UCSRC_BIT_USBS);
    #endif

    /*Character Size 8-bit*/
    u8TempLoc = (u8TempLoc & UCSRC_UCSZ01_MASK) | (USART_CHARACTER_SIZE<<UCSRC_BIT_UCSZ0);

    /*Clock Polarity Rising/Falling Transmission*/
    #if USART_CLK_POLARITY == USART_CLK_POLARITY_RISING_TRANSMIT
    CLR_BIT(u8TempLoc, UCSRC_BIT_UCPOL);
    #elif USART_CLK_POLARITY == USART_CLK_POLARITY_FALLING_TRANSMIT
    SET_BIT(u8TempLoc, UCSRC_BIT_UCPOL);
    #endif

    /*Update UCSRC*/
    UCSRC = u8TempLoc;

    /*****UBRRH*****/
    u8TempLoc = 0;
    /*UBRRH Select*/
    CLR_BIT(u8TempLoc, UBRRH_BIT_URSEL);

    /*Baud Rate High bits*/
    u8TempLoc |= (USART_BAUD_RATE>>8);

    /*Update UBRRH*/
    UBRRH = u8TempLoc;

    /*****UBBRL*****/
    /*Baud Rate low bits*/
    UBRRL = USART_BAUD_RATE;

    /*****UCSRB*****/
    /*RX Complete Interrupt Enable/Disable*/
    #if USART_RX_INTERRUPT == USART_RX_INTERRUPT_DISABLE
    CLR_BIT(UCSRB, UCSRB_BIT_RXCIE);
    #elif USART_RX_INTERRUPT == USART_RX_INTERRUPT_ENABLE
    SET_BIT(UCSRB, UCSRB_BIT_RXCIE);
    #endif

    /*TX Complete Interrupt Enable/Disable*/
    #if USART_TX_INTERRUPT == USART_TX_INTERRUPT_DISABLE
    CLR_BIT(UCSRB, UCSRB_BIT_TXCIE);
    #elif USART_TX_INTERRUPT == USART_TX_INTERRUPT_ENABLE
    SET_BIT(UCSRB, UCSRB_BIT_TXCIE);
    #endif

    /*Data Reg. empty Interrupt Enable/Disable*/
    #if USART_DATA_EMPTY_INTERRUPT == USART_DATA_EMPTY_INTERRUPT_DISABLE
    CLR_BIT(UCSRB, UCSRB_BIT_UDRIE);
    #elif USART_DATA_EMPTY_INTERRUPT == USART_DATA_EMPTY_INTERRUPT_ENABLE
    SET_BIT(UCSRB, UCSRB_BIT_UDRIE);
    #endif

    /*Receiver Enable/Disable*/
    #if USART_RECEIVER == USART_RECEIVER_DISABLE
    CLR_BIT(UCSRB, UCSRB_BIT_RXEN);
    #elif USART_RECEIVER == USART_RECEIVER_ENABLE
    SET_BIT(UCSRB, UCSRB_BIT_RXEN);
    #endif

    /*Transmitter Enable/Disable*/
    #if USART_TRANSMITTER == USART_TRANSMITTER_DISABLE
    CLR_BIT(UCSRB, UCSRB_BIT_TXEN);
    #elif USART_TRANSMITTER == USART_TRANSMITTER_ENABLE
    SET_BIT(UCSRB, UCSRB_BIT_TXEN);
    #endif
}

tenuErrorStatus USART_enuReceiveSynch(uint16* const pu16DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pu16DataCpy==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        while(GET_BIT(UCSRA, UCSRA_BIT_RXC)==0);

        *pu16DataCpy = 0;

        #if USART_CHARACTER_SIZE == USART_CHARACTER_SIZE_9BIT
        *pu16DataCpy |= (GET_BIT(UCSRB, UCSRB_BIT_RXB8)<<8);
        #endif

        *pu16DataCpy |= UDR;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus USART_enuTransmitSynch(const uint16 u16DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(u16DataCpy>USART_DATA_MAX_VALUE_9BIT)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        while(GET_BIT(UCSRA, UCSRA_BIT_UDRE)==0);

        #if USART_CHARACTER_SIZE == USART_CHARACTER_SIZE_9BIT
        UCSRB |= (GET_BIT(u16DataCpy, 8)<<UCSRB_BIT_TXB8);
        #endif

        UDR = u16DataCpy;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus USART_enuGetData(uint16* const pu16DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;
    uint8* pu8DataLoc = (uint8*) pu16DataCpy;

    if(pu8DataLoc==NULL_PTR)
    {
        enuErrorStatLoc = E_NOK_NULL_POINTER;
    }
    else
    {
        *pu8DataLoc = 0;

        #if USART_CHARACTER_SIZE == USART_CHARACTER_SIZE_9BIT
        *pu16DataCpy = 0;
        *pu16DataCpy |= (GET_BIT(UCSRB, UCSRB_BIT_RXB8)<<8);
		#else
        *((uint8*)pu16DataCpy)=0;
        #endif

        *pu8DataLoc |= UDR;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus USART_enuSetData(const uint16 u16DataCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(u16DataCpy>USART_DATA_MAX_VALUE_9BIT)
    {
        enuErrorStatLoc = E_NOK_PARAMETER_OUT_OF_RANGE;
    }
    else
    {
        #if USART_CHARACTER_SIZE == USART_CHARACTER_SIZE_9BIT
        UCSRB |= (GET_BIT(u16DataCpy, 8)<<UCSRB_BIT_TXB8);
        #endif
        UDR = u16DataCpy;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus USART_enuReceiveSetCallBack(const pf pfReceiveISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfReceiveISRCpy!=NULL_PTR)
    {
        pfReceiveISRGlb = pfReceiveISRCpy;
    }

    return enuErrorStatLoc;
}

tenuErrorStatus USART_enuTransmitSetCallBack(const pf pfTransmitISRCpy)
{
    tenuErrorStatus enuErrorStatLoc = E_OK;

    if(pfTransmitISRCpy!=NULL_PTR)
    {
        pfTransmitISRGlb = pfTransmitISRCpy;
    }

    return enuErrorStatLoc;
}

/*RXC*/
void __vector_13 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_13 (void){
    if(pfReceiveISRGlb!=NULL_PTR)
    {
        pfReceiveISRGlb();
    }
}

/*TXC*/
void __vector_15 (void) __attribute__ ((signal,used, externally_visible)) ; \
void __vector_15 (void){
    if(pfTransmitISRGlb!=NULL_PTR)
    {
        pfTransmitISRGlb();
    }
}
