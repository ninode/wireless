/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void CORE_TIMER_InterruptHandler( void );
void TIMER_2_InterruptHandler( void );
void NVM_InterruptHandler( void );
void UART1_FAULT_InterruptHandler( void );
void UART1_RX_InterruptHandler( void );
void UART1_TX_InterruptHandler( void );
void WDRV_PIC32MZW_TasksRFSMCISR( void );
void WDRV_PIC32MZW_TasksRFMACISR( void );
void WDRV_PIC32MZW_TasksRFTimer0ISR( void );
void qtm_pic32_cvd_handler_eoc( void );



/* All the handlers are defined here.  Each will call its PLIB-specific function. */
void __ISR(_CORE_TIMER_VECTOR, ipl1SRS) CORE_TIMER_Handler (void)
{
    CORE_TIMER_InterruptHandler();
}

void __ISR(_TIMER_2_VECTOR, ipl1SRS) TIMER_2_Handler (void)
{
    TIMER_2_InterruptHandler();
}

void __ISR(_FLASH_CONTROL_VECTOR, ipl1SRS) FLASH_CONTROL_Handler (void)
{
    NVM_InterruptHandler();
}

void __ISR(_UART1_FAULT_VECTOR, ipl1SRS) UART1_FAULT_Handler (void)
{
    UART1_FAULT_InterruptHandler();
}

void __ISR(_UART1_RX_VECTOR, ipl1SRS) UART1_RX_Handler (void)
{
    UART1_RX_InterruptHandler();
}

void __ISR(_UART1_TX_VECTOR, ipl1SRS) UART1_TX_Handler (void)
{
    UART1_TX_InterruptHandler();
}

void __ISR(_RFSMC_VECTOR, ipl1SRS) RFSMC_Handler (void)
{
    WDRV_PIC32MZW_TasksRFSMCISR();
}

void __ISR(_RFMAC_VECTOR, ipl1SRS) RFMAC_Handler (void)
{
    WDRV_PIC32MZW_TasksRFMACISR();
}

void __ISR(_RFTM0_VECTOR, ipl1SRS) RFTM0_Handler (void)
{
    WDRV_PIC32MZW_TasksRFTimer0ISR();
}

void __ISR(_CVD_EVENT_VECTOR, ipl1SRS) CVD_EVENT_Handler (void)
{
    qtm_pic32_cvd_handler_eoc();
}




/*******************************************************************************
 End of File
*/
