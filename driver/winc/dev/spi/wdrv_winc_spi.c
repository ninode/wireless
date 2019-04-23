/*******************************************************************************
  WINC Wireless Driver SPI Communication Support

  File Name:
    wdrv_winc_spi.c

  Summary:
    WINC Wireless Driver SPI Communications Support

  Description:
    Supports SPI communications to the WINC module.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/

#include "configuration.h"
#include "definitions.h"
#include "osal/osal.h"

#include "wdrv_winc_common.h"

#if defined(__PIC32MZ__)
#define WDRV_DCACHE_CLEAN(addr, size) _DataCacheClean(addr, size)
#else /* !defined(__PIC32MZ__) */
#define WDRV_DCACHE_CLEAN(addr, size) do { } while (0)
#endif /* defined(__PIC32MZ__) */

#if (DRV_SPI_DMA != 0)
#define SPI_DMA_DCACHE_CLEAN(addr, size) WDRV_DCACHE_CLEAN(addr, size)
#define SPI_DMA_MAX_TX_SIZE DRV_SPI_DMA_TXFER_SIZE
#define SPI_DMA_MAX_RX_SIZE DRV_SPI_DMA_DUMMY_BUFFER_SIZE
#else /* (DRV_SPI_DMA != 0) */
#define SPI_DMA_DCACHE_CLEAN(addr, size) do { } while (0)
#endif /* (DRV_SPI_DMA != 0) */

static DRV_HANDLE spiHandle = DRV_HANDLE_INVALID;
static OSAL_SEM_HANDLE_TYPE txSyncSem;
static OSAL_SEM_HANDLE_TYPE rxSyncSem;

#if defined(__PIC32MZ__)
static void _DataCacheClean(unsigned char *address, uint32_t size)
{
    if (IS_KVA0(address))
    {
        uint32_t a = (uint32_t)address & 0xfffffff0;
        uint32_t r = (uint32_t)address & 0x0000000f;
        uint32_t s = ((size + r + 15) >> 4) << 4;

        SYS_DEVCON_DataCacheClean(a, s);
    }
}
#endif /* defined(__PIC32MZ__) */
static DRV_SPI_TRANSFER_HANDLE transferTxHandle;
static DRV_SPI_TRANSFER_HANDLE transferRxHandle; //aa

/* ramya 
static void _SPI_TxComplete(DRV_SPI_BUFFER_EVENT event, DRV_SPI_BUFFER_HANDLE bufferHandle, void *context )
{
    OSAL_SEM_PostISR(&txSyncSem);
}
 */
/** ramya 
static void _SPI_RxComplete(DRV_SPI_BUFFER_EVENT event, DRV_SPI_BUFFER_HANDLE bufferHandle, void *context )
{
    OSAL_SEM_PostISR(&rxSyncSem);
}
*/
static bool _SPI_Tx(unsigned char *buf, uint32_t size)
{
    SPI_DMA_DCACHE_CLEAN(buf, size);
    DRV_SPI_WriteTransferAdd(spiHandle, buf, size, &transferTxHandle);

    if(transferTxHandle == DRV_SPI_TRANSFER_HANDLE_INVALID)
    {
        //printf("sagar:_SPI_Tx Error handling");
        // Error handling here
    }
    
    while (OSAL_RESULT_FALSE == OSAL_SEM_Pend(&txSyncSem, OSAL_WAIT_FOREVER))
    {
        
    }
    
#if 0
    SPI_DMA_DCACHE_CLEAN(buf, size);
    if (DRV_SPI_BUFFER_HANDLE_INVALID == DRV_SPI_BufferAddWrite(spiHandle, buf, size, _SPI_TxComplete, 0))
    {
        return false;
    }

    while (OSAL_RESULT_FALSE == OSAL_SEM_Pend(&txSyncSem, OSAL_WAIT_FOREVER))
    {
    }
#endif
    return true;
}

static bool _SPI_Rx(unsigned char *const buf, uint32_t size)
{
    SPI_DMA_DCACHE_CLEAN(buf, size);
    
    DRV_SPI_ReadTransferAdd(spiHandle, buf, size, &transferRxHandle);
        
    if(transferRxHandle == DRV_SPI_TRANSFER_HANDLE_INVALID)
    {
        //printf("sagar:_SPI_Rx Error handling");
        // Error handling here  
    }
    while (OSAL_RESULT_FALSE == OSAL_SEM_Pend(&rxSyncSem, OSAL_WAIT_FOREVER))
    {
    }

#if 0 // sagar : commecnted
    if (DRV_SPI_BUFFER_HANDLE_INVALID == DRV_SPI_BufferAddRead(spiHandle, buf, size, _SPI_RxComplete, 0))
    {
        return false;
    }

    while (OSAL_RESULT_FALSE == OSAL_SEM_Pend(&rxSyncSem, OSAL_WAIT_FOREVER))
    {
    }
#endif
    return true;
}

/****************************************************************************
 * Function:        WDRV_WINC_SPISend
 * Summary: Sends data out to the module through the SPI bus.
 *****************************************************************************/
bool WDRV_WINC_SPISend(unsigned char *const buf, uint32_t size)
{
    bool ret = true;
    unsigned char *pData;

    pData = buf;
    SYS_PORT_PinClear(PORT_PIN_PA05); // sercom0 spi_ss pin is PA05
    //ramya
    //SYS_PORTS_PinClear(PORTS_ID_0, WDRV_WINC_SPI_SSN_PORT, WDRV_WINC_SPI_SSN_PIN);

#if (DRV_SPI_DMA != 0)
    while ((true == ret) && (size > SPI_DMA_MAX_TX_SIZE))
    {
        ret = _SPI_Tx(pData, SPI_DMA_MAX_TX_SIZE);
        size -= SPI_DMA_MAX_TX_SIZE;
        pData += SPI_DMA_MAX_TX_SIZE;
    }
#endif

    if ((true == ret) && (size > 0))
    {
        ret = _SPI_Tx(pData, size);
    }
    SYS_PORT_PinSet(PORT_PIN_PA05); // sercom0 spi_ss pin is PA05
    //ramya
    //SYS_PORTS_PinSet(PORTS_ID_0, WDRV_WINC_SPI_SSN_PORT, WDRV_WINC_SPI_SSN_PIN);

    return ret;
}

/****************************************************************************
 * Function:        WDRV_WINC_SPIReceive
 * Summary: Receives data from the module through the SPI bus.
 *****************************************************************************/
bool WDRV_WINC_SPIReceive(unsigned char *const buf, uint32_t size)
{
    bool ret = true;
    unsigned char *pData;

    pData = buf;
    SYS_PORT_PinClear(PORT_PIN_PA05); // sercom0 spi_ss pin is PA05
    //ramya
    //SYS_PORTS_PinClear(PORTS_ID_0, WDRV_WINC_SPI_SSN_PORT, WDRV_WINC_SPI_SSN_PIN);

#if (DRV_SPI_DMA != 0)
    while ((true == ret) && (size > SPI_DMA_MAX_RX_SIZE))
    {
        ret = _SPI_Rx(pData, SPI_DMA_MAX_RX_SIZE);
        size -= SPI_DMA_MAX_RX_SIZE;
        pData += SPI_DMA_MAX_RX_SIZE;
    }
#endif

    if ((true == ret) && (size > 0))
    {
        ret = _SPI_Rx(pData, size);
    }
    SYS_PORT_PinSet(PORT_PIN_PA05); // sercom0 spi_ss pin is PA05
    //ramya
    //SYS_PORTS_PinSet(PORTS_ID_0, WDRV_WINC_SPI_SSN_PORT, WDRV_WINC_SPI_SSN_PIN);

    return ret;
}


void APP_SPITransferEventHandler(DRV_SPI_TRANSFER_EVENT event,
        DRV_SPI_TRANSFER_HANDLE handle, uintptr_t context)
{
    // The context handle was set to an application specific
    // object. It is now retrievable easily in the event handler.
    // sagar : commented bcz of error
   // MY_APP_OBJ myAppObj = (MY_APP_OBJ *) context;

    switch(event)
    {
        case DRV_SPI_TRANSFER_EVENT_COMPLETE:
            // This means the data was transferred.
            if(transferTxHandle == handle)
             OSAL_SEM_PostISR(&txSyncSem);
            else if(transferRxHandle == handle)
             OSAL_SEM_PostISR(&rxSyncSem);
            break;

        case DRV_SPI_TRANSFER_EVENT_ERROR:
            // Error handling here.
            break;

        default:
            break;
    }
}
/****************************************************************************
 * Function:        WDRV_WINC_SPIInitialize
 * Summary: Initializes the SPI object for the WiFi driver.
 *****************************************************************************/
void WDRV_WINC_SPIInitialize(void)
{
    if (OSAL_RESULT_TRUE != OSAL_SEM_Create(&txSyncSem, OSAL_SEM_TYPE_COUNTING, 10, 0))
    {
        return;
    }

    if (OSAL_RESULT_TRUE != OSAL_SEM_Create(&rxSyncSem, OSAL_SEM_TYPE_COUNTING, 10, 0))
    {
        return;
    }

    if (DRV_HANDLE_INVALID == spiHandle)
    {
        spiHandle = DRV_SPI_Open(WDRV_WINC_SPI_INDEX, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_BLOCKING);
        if (DRV_HANDLE_INVALID == spiHandle)
        {
            WDRV_DBG_ERROR_PRINT("SPI init failed\r\n");
        }
    }
    DRV_SPI_TransferEventHandlerSet( spiHandle, APP_SPITransferEventHandler,
                                 0 );

}

//DOM-IGNORE-END
