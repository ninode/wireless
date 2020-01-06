/*******************************************************************************
  PIC32MZW Driver Authentication Context Header File

  Company:
    Microchip Technology Inc.

  File Name:
    wdrv_pic32mzw_authctx.h

  Summary:
    PIC32MZW wireless driver authentication context header file.

  Description:
    This interface manages the authentication contexts which 'wrap' the state
      associated with authentication schemes.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc. All rights reserved.

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
// DOM-IGNORE-END

#ifndef _WDRV_PIC32MZW_AUTHCTX_H
#define _WDRV_PIC32MZW_AUTHCTX_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>

#include "system_config.h"
#include "system_definitions.h"
#include "wdrv_pic32mzw_common.h"

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Driver Authentication Context Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*  Authentication Types

  Summary:
    List of possible authentication types.

  Description:
    This type defines the possible authentication types that can be requested
    in AP mode or STA mode.

  Remarks:
    None.
*/

typedef enum
{
    /* This type may be used in helper function
     * WDRV_PIC32MZW_AuthCtxSetPersonal, in which case it is translated into
     * the best practice auth type. Other uses of this type are invalid. */
    WDRV_PIC32MZW_AUTH_TYPE_DEFAULT,

    /* No encryption. */
    WDRV_PIC32MZW_AUTH_TYPE_OPEN,

    /* WEP encryption. */
    WDRV_PIC32MZW_AUTH_TYPE_WEP,

    /* WPA2 mixed mode (AP) / compatibility mode (STA) with PSK.
     * (As an AP GTK is TKIP, as a STA GTK is chosen by AP;
     * PTK can be CCMP or TKIP) */
    WDRV_PIC32MZW_AUTH_TYPE_WPAWPA2_PERSONAL,

    /* WPA2-only authentication with PSK. (PTK and GTK are both CCMP).       */
    /* Note that a WPA2-only STA will not connect to a WPA2 mixed mode AP.   */
    WDRV_PIC32MZW_AUTH_TYPE_WPA2_PERSONAL,

    /* WPA3 SAE transition mode. (CCMP, IGTK can be BIP or none) */
    WDRV_PIC32MZW_AUTH_TYPE_WPA2WPA3_PERSONAL,

    /* WPA3 SAE authentication. (CCMP, IGTK is BIP) */
    WDRV_PIC32MZW_AUTH_TYPE_WPA3_PERSONAL,

    /* Authentication types with this value or above are not recognised. */
    WDRV_PIC32MZW_AUTH_TYPE_MAX
} WDRV_PIC32MZW_AUTH_TYPE;

// *****************************************************************************
/*  Authentication Modifiers

  Summary:
    List of possible authentication modifiers.

  Description:
    This type defines the possible modifications that can be made to the
    authentication types in WDRV_PIC32MZW_AUTH_TYPE.

  Remarks:
    None.
*/

typedef enum
{
    WDRV_PIC32MZW_AUTH_MOD_NONE     = 0,
    /* Require protection of management frames.
     * Not valid with Open, WEP or WPAWPA2 auth types.
     * Ignored for WPA3-only auth types (PMF is mandatory in WPA3). */
    WDRV_PIC32MZW_AUTH_MOD_MFPR     = 0x01,
} WDRV_PIC32MZW_AUTH_MOD_MASK;

// *****************************************************************************
/*  Authentication Context

  Summary:
    Context structure containing information about authentication types.

  Description:
    The context contains the type of authentication as well as any state
      information.

  Remarks:
    None.
*/

typedef struct
{
    /* Authentication type of context. */
    WDRV_PIC32MZW_AUTH_TYPE authType;

    /* Authentication modifiers. */
    WDRV_PIC32MZW_AUTH_MOD_MASK authMod;

    /* Union of data structures for each authentication type. */
    union
    {
        /* WEP authentication state. */
        struct
        {
            /* The WEP key index in the range 1-4. */
            uint8_t idx;
            /* The WEP key size is 10 for WEP_40 and 26 for WEP_104. */
            uint8_t size;
            /* The WEP key. */
            uint8_t key[WDRV_PIC32MZW_WEP_104_KEY_STRING_SIZE+1];
        } WEP;

        /* WPA-Personal authentication state. */
        struct
        {
            /* The size of the password or PSK, in characters/bytes          */
            /* Size must be in the range 8 to 63 for a WPA(2) password.      */
            /* Size must be 64 for a WPA(2) PSK.                             */
            uint8_t size;
            /* The password or PSK. */
            uint8_t password[WDRV_PIC32MZW_PSK_LEN];
        } personal;
    } authInfo;
} WDRV_PIC32MZW_AUTH_CONTEXT;

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Driver Authentication Context Routines
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    bool WDRV_PIC32MZW_AuthCtxIsValid(const WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx)

  Summary:
    Tests if an authentication context is valid.

  Description:
    Tests the elements of the authentication context to judge if their values are legal.

  Precondition:
    None.

  Parameters:
    pAuthCtx  - Pointer to an authentication context.

  Returns:
    true or false indicating if context is valid.

  Remarks:
    None.

*/

bool WDRV_PIC32MZW_AuthCtxIsValid(const WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetDefaults
    (
        WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx
    )

  Summary:
    Configures an authentication context into a default state.

  Description:
    Ensures that each element of the structure is configured into a default state.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.

  Returns:
    WDRV_PIC32MZW_STATUS_OK          - The context has been configured.
    WDRV_PIC32MZW_STATUS_INVALID_ARG - The parameters were incorrect.

  Remarks:
    A default context is not valid until it is configured.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetDefaults
(
    WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx
);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetOpen
    (
        WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx
    )

  Summary:
    Configure an authentication context for Open authentication.

  Description:
    The type and state information are configured appropriately for Open
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The context has been configured.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetOpen
(
    WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx
);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetWEP
    (
        WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
        uint8_t idx,
        uint8_t *pKey,
        uint8_t size
    )

  Summary:
    Configure an authentication context for WEP authentication.

  Description:
    The type and state information are configured appropriately for WEP
      authentication.

  Precondition:
    None.

  Parameters:
    pAuthCtx - Pointer to an authentication context.
    idx      - WEP index.
    pKey     - Pointer to WEP key.
    size     - Size of WEP key.

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The context has been configured.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.

  Remarks:
    None.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetWEP
(
    WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
    uint8_t idx,
    uint8_t *const pKey,
    uint8_t size
);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetPersonal
    (
        WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
        uint8_t *pPassword,
        uint8_t size,
        WDRV_PIC32MZW_AUTH_TYPE authType
    )

  Summary:
    Configure an authentication context for WPA personal authentication.

  Description:
    The type and state information are configured appropriately for personal
      authentication (WPA2 or WPA2 mixed/compatibility mode) with the password
      or PSK provided.

  Precondition:
    None.

  Parameters:
    pAuthCtx    - Pointer to an authentication context.
    pPassword   - Pointer to password (or 64-character PSK).
    size        - Size of password (or 64 for PSK).
    authType    - Authentication type (or WDRV_PIC32MZW_AUTH_TYPE_DEFAULT).

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The context has been configured.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.
*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetPersonal
(
    WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
    uint8_t *const pPassword,
    uint8_t size,
    WDRV_PIC32MZW_AUTH_TYPE authType
);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetMfpRequired
    (
        WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
        bool isRequired
    )

  Summary:
    Configure the Management Frame Protection Required bit of an authentication
    context.

  Description:
    The Management Frame Protection Required bit of the authentication context
    is set/cleared according to the isRequired parameter.

  Precondition:
    None.

  Parameters:
    pAuthCtx    - Pointer to an authentication context.
    isRequired  - True to set the Management Frame Protection Required bit.
                  False to clear the Management Frame Protection Required bit.

  Returns:
    WDRV_PIC32MZW_STATUS_OK             - The context has been modified.
    WDRV_PIC32MZW_STATUS_INVALID_ARG    - The parameters were incorrect.
*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_AuthCtxSetMfpRequired
(
    WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
    bool isRequired
);

#endif /* _WDRV_PIC32MZW_AUTHCTX_H */
