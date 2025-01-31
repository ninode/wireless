/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for GPIO_RA4 pin ***/
#define GPIO_RA4_Set()               (LATASET = (1<<4))
#define GPIO_RA4_Clear()             (LATACLR = (1<<4))
#define GPIO_RA4_Toggle()            (LATAINV= (1<<4))
#define GPIO_RA4_OutputEnable()      (TRISACLR = (1<<4))
#define GPIO_RA4_InputEnable()       (TRISASET = (1<<4))
#define GPIO_RA4_Get()               ((PORTA >> 4) & 0x1)
#define GPIO_RA4_PIN                  GPIO_PIN_RA4

/*** Macros for GPIO_RK6 pin ***/
#define GPIO_RK6_Set()               (LATKSET = (1<<6))
#define GPIO_RK6_Clear()             (LATKCLR = (1<<6))
#define GPIO_RK6_Toggle()            (LATKINV= (1<<6))
#define GPIO_RK6_OutputEnable()      (TRISKCLR = (1<<6))
#define GPIO_RK6_InputEnable()       (TRISKSET = (1<<6))
#define GPIO_RK6_Get()               ((PORTK >> 6) & 0x1)
#define GPIO_RK6_PIN                  GPIO_PIN_RK6

/*** Macros for GPIO_RA11 pin ***/
#define GPIO_RA11_Set()               (LATASET = (1<<11))
#define GPIO_RA11_Clear()             (LATACLR = (1<<11))
#define GPIO_RA11_Toggle()            (LATAINV= (1<<11))
#define GPIO_RA11_OutputEnable()      (TRISACLR = (1<<11))
#define GPIO_RA11_InputEnable()       (TRISASET = (1<<11))
#define GPIO_RA11_Get()               ((PORTA >> 11) & 0x1)
#define GPIO_RA11_PIN                  GPIO_PIN_RA11

/*** Macros for GPIO_RB12 pin ***/
#define GPIO_RB12_Set()               (LATBSET = (1<<12))
#define GPIO_RB12_Clear()             (LATBCLR = (1<<12))
#define GPIO_RB12_Toggle()            (LATBINV= (1<<12))
#define GPIO_RB12_OutputEnable()      (TRISBCLR = (1<<12))
#define GPIO_RB12_InputEnable()       (TRISBSET = (1<<12))
#define GPIO_RB12_Get()               ((PORTB >> 12) & 0x1)
#define GPIO_RB12_PIN                  GPIO_PIN_RB12

/*** Macros for GPIO_RB7 pin ***/
#define GPIO_RB7_Set()               (LATBSET = (1<<7))
#define GPIO_RB7_Clear()             (LATBCLR = (1<<7))
#define GPIO_RB7_Toggle()            (LATBINV= (1<<7))
#define GPIO_RB7_OutputEnable()      (TRISBCLR = (1<<7))
#define GPIO_RB7_InputEnable()       (TRISBSET = (1<<7))
#define GPIO_RB7_Get()               ((PORTB >> 7) & 0x1)
#define GPIO_RB7_PIN                  GPIO_PIN_RB7

/*** Macros for GPIO_RA5 pin ***/
#define GPIO_RA5_Set()               (LATASET = (1<<5))
#define GPIO_RA5_Clear()             (LATACLR = (1<<5))
#define GPIO_RA5_Toggle()            (LATAINV= (1<<5))
#define GPIO_RA5_OutputEnable()      (TRISACLR = (1<<5))
#define GPIO_RA5_InputEnable()       (TRISASET = (1<<5))
#define GPIO_RA5_Get()               ((PORTA >> 5) & 0x1)
#define GPIO_RA5_PIN                  GPIO_PIN_RA5

/*** Macros for GPIO_RK4 pin ***/
#define GPIO_RK4_Set()               (LATKSET = (1<<4))
#define GPIO_RK4_Clear()             (LATKCLR = (1<<4))
#define GPIO_RK4_Toggle()            (LATKINV= (1<<4))
#define GPIO_RK4_OutputEnable()      (TRISKCLR = (1<<4))
#define GPIO_RK4_InputEnable()       (TRISKSET = (1<<4))
#define GPIO_RK4_Get()               ((PORTK >> 4) & 0x1)
#define GPIO_RK4_PIN                  GPIO_PIN_RK4

/*** Macros for GPIO_RK5 pin ***/
#define GPIO_RK5_Set()               (LATKSET = (1<<5))
#define GPIO_RK5_Clear()             (LATKCLR = (1<<5))
#define GPIO_RK5_Toggle()            (LATKINV= (1<<5))
#define GPIO_RK5_OutputEnable()      (TRISKCLR = (1<<5))
#define GPIO_RK5_InputEnable()       (TRISKSET = (1<<5))
#define GPIO_RK5_Get()               ((PORTK >> 5) & 0x1)
#define GPIO_RK5_PIN                  GPIO_PIN_RK5


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
    GPIO_PORT_K = 3,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RA5 = 5,
    GPIO_PIN_RA6 = 6,
    GPIO_PIN_RA7 = 7,
    GPIO_PIN_RA8 = 8,
    GPIO_PIN_RA9 = 9,
    GPIO_PIN_RA10 = 10,
    GPIO_PIN_RA11 = 11,
    GPIO_PIN_RA12 = 12,
    GPIO_PIN_RA13 = 13,
    GPIO_PIN_RA14 = 14,
    GPIO_PIN_RA15 = 15,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB6 = 22,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB12 = 28,
    GPIO_PIN_RB13 = 29,
    GPIO_PIN_RB14 = 30,
    GPIO_PIN_RC0 = 32,
    GPIO_PIN_RC1 = 33,
    GPIO_PIN_RC2 = 34,
    GPIO_PIN_RC3 = 35,
    GPIO_PIN_RC4 = 36,
    GPIO_PIN_RC5 = 37,
    GPIO_PIN_RC6 = 38,
    GPIO_PIN_RC7 = 39,
    GPIO_PIN_RC8 = 40,
    GPIO_PIN_RC9 = 41,
    GPIO_PIN_RC10 = 42,
    GPIO_PIN_RC11 = 43,
    GPIO_PIN_RC12 = 44,
    GPIO_PIN_RC13 = 45,
    GPIO_PIN_RC14 = 46,
    GPIO_PIN_RC15 = 47,
    GPIO_PIN_RK0 = 48,
    GPIO_PIN_RK1 = 49,
    GPIO_PIN_RK2 = 50,
    GPIO_PIN_RK3 = 51,
    GPIO_PIN_RK4 = 52,
    GPIO_PIN_RK5 = 53,
    GPIO_PIN_RK6 = 54,
    GPIO_PIN_RK7 = 55,
    GPIO_PIN_RK8 = 56,
    GPIO_PIN_RK9 = 57,
    GPIO_PIN_RK10 = 58,
    GPIO_PIN_RK11 = 59,
    GPIO_PIN_RK12 = 60,
    GPIO_PIN_RK13 = 61,
    GPIO_PIN_RK14 = 62,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
