/*******************************************************************************
  WINC Driver TLS/SSL Implementation

  File Name:
    wdrv_winc_ssl.c

  Summary:
    WINC wireless driver TLS/SSL implementation.

  Description:
    Provides an interface to configure TLS/SSL support.
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "wdrv_winc.h"
#include "wdrv_winc_common.h"
#include "wdrv_winc_ssl.h"
#include "socket.h"
#include "m2m_ssl.h"

// *****************************************************************************
// *****************************************************************************
// Section: WINC Driver TLS/SSL Global Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*  Cipher Suite

  Summary:
    Link cipher suite ID to bit mask.

  Description:
    Associates a cipher suite ID with a bit mask.

  Remarks:
    None.
*/

typedef struct
{
    uint16_t ianaID;
    uint16_t bitMask;
} WDRV_WINC_CIPHER_SUITE_MAP;

/* This table maps all the supported cipher suites to a bit mask. */
#define WDRV_WINC_NUM_CIPHER_SUITES     16
static const WDRV_WINC_CIPHER_SUITE_MAP _cipherSuiteMap[WDRV_WINC_NUM_CIPHER_SUITES] =
{
    {
        .ianaID  = WDRV_WINC_TLS_RSA_WITH_AES_128_CBC_SHA,
        .bitMask = SSL_CIPHER_RSA_WITH_AES_128_CBC_SHA
    },
    {
        .ianaID  = WDRV_WINC_TLS_RSA_WITH_AES_128_CBC_SHA256,
        .bitMask = SSL_CIPHER_RSA_WITH_AES_128_CBC_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_DHE_RSA_WITH_AES_128_CBC_SHA,
        .bitMask = SSL_CIPHER_DHE_RSA_WITH_AES_128_CBC_SHA
    },
    {
        .ianaID  = WDRV_WINC_TLS_DHE_RSA_WITH_AES_128_CBC_SHA256,
        .bitMask = SSL_CIPHER_DHE_RSA_WITH_AES_128_CBC_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_RSA_WITH_AES_128_GCM_SHA256,
        .bitMask = SSL_CIPHER_RSA_WITH_AES_128_GCM_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256,
        .bitMask = SSL_CIPHER_DHE_RSA_WITH_AES_128_GCM_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_RSA_WITH_AES_256_CBC_SHA,
        .bitMask = SSL_CIPHER_RSA_WITH_AES_256_CBC_SHA
    },
    {
        .ianaID  = WDRV_WINC_TLS_RSA_WITH_AES_256_CBC_SHA256,
        .bitMask = SSL_CIPHER_RSA_WITH_AES_256_CBC_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_DHE_RSA_WITH_AES_256_CBC_SHA,
        .bitMask = SSL_CIPHER_DHE_RSA_WITH_AES_256_CBC_SHA
    },
    {
        .ianaID  = WDRV_WINC_TLS_DHE_RSA_WITH_AES_256_CBC_SHA256,
        .bitMask = SSL_CIPHER_DHE_RSA_WITH_AES_256_CBC_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,
        .bitMask = SSL_CIPHER_ECDHE_RSA_WITH_AES_128_CBC_SHA
    },
    {
        .ianaID  = WDRV_WINC_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA,
        .bitMask = SSL_CIPHER_ECDHE_RSA_WITH_AES_256_CBC_SHA
    },
    {
        .ianaID  = WDRV_WINC_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
        .bitMask = SSL_CIPHER_ECDHE_RSA_WITH_AES_128_CBC_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
        .bitMask = SSL_CIPHER_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
        .bitMask = SSL_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256
    },
    {
        .ianaID  = WDRV_WINC_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
        .bitMask = SSL_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
    }
};

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_SSLCTXDefault
    (
        WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx
    )

  Summary:
    Initialize an SSL cipher suite context.

  Description:
    Sets the default values for the cipher suite context.

  Remarks:
    See wdrv_winc_ssl.h for usage information.

*/

WDRV_WINC_STATUS WDRV_WINC_SSLCTXDefault
(
    WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx
)
{
    /* Ensure the cipher suite context is valid. */
    if (NULL == pSSLCipherSuiteCtx)
    {
        return WDRV_WINC_STATUS_INVALID_ARG;
    }

    /* Set the default cipher suites. */
#if defined(WDRV_WINC_DEVICE_WINC1500)
    pSSLCipherSuiteCtx->ciperSuites = 0;
#elif defined(WDRV_WINC_DEVICE_WINC3400)
    pSSLCipherSuiteCtx->ciperSuites = SSL_DEFAULT_CIPHERS;
#endif

    return WDRV_WINC_STATUS_OK;
}

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_SSLCTXCipherSuitesSet
    (
        WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx,
        uint16_t *pCipherSuiteList,
        uint8_t numCipherSuites
    )

  Summary:
    Set the cipher suites enabled in a context.

  Description:
    Sets the list of enabled cipher suites within a cipher suite context using
      IANA defined IDs.

  Remarks:
    See wdrv_winc_ssl.h for usage information.

*/

WDRV_WINC_STATUS WDRV_WINC_SSLCTXCipherSuitesSet
(
    WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx,
    uint16_t *pCipherSuiteList,
    uint8_t numCipherSuites
)
{
    uint8_t i;

    /* Ensure the cipher suite context and cipher suite list are valid. */
    if ((NULL == pSSLCipherSuiteCtx) || (NULL == pCipherSuiteList) || (numCipherSuites > 16))
    {
        return WDRV_WINC_STATUS_INVALID_ARG;
    }

    /* Add each cipher suite in the list to the supported cipher suite bit mask
       by searching the table which maps ID to bit mask. */
    pSSLCipherSuiteCtx->ciperSuites = 0;

    while((numCipherSuites--) && (*pCipherSuiteList != WDRV_WINC_TLS_NULL_WITH_NULL_NULL))
    {
        for (i=0; i<WDRV_WINC_NUM_CIPHER_SUITES; i++)
        {
            if (_cipherSuiteMap[i].ianaID == *pCipherSuiteList)
            {
                pSSLCipherSuiteCtx->ciperSuites |= _cipherSuiteMap[i].bitMask;
            }
        }

        pCipherSuiteList++;
    }

    return WDRV_WINC_STATUS_OK;
}

//*******************************************************************************
/*
  Function:
    uint8_t WDRV_WINC_SSLCTXCipherSuitesGet
    (
        WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx,
        uint16_t *pCipherSuiteList,
        uint8_t maxNumCipherSuites
    )

  Summary:
    Returns the enabled cipher suites from a context.

  Description:
    Gets the list of cipher suites from a context as a list of IANA IDs

  Remarks:
    See wdrv_winc_ssl.h for usage information.

*/

uint8_t WDRV_WINC_SSLCTXCipherSuitesGet
(
    WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx,
    uint16_t *pCipherSuiteList,
    uint8_t maxNumCipherSuites
)
{
    uint8_t i;
    uint32_t cipherMask;
    uint8_t numCipherSuites;

    /* Ensure the cipher suite context and cipher suite list are valid. */
    if ((NULL == pSSLCipherSuiteCtx) || (NULL == pCipherSuiteList))
    {
        return 0;
    }

    cipherMask = 0x0001;
    numCipherSuites = 0;

    /* Translate the cipher suite bit mask back into a cipher suite list. */
    while ((0 != cipherMask) && (numCipherSuites < maxNumCipherSuites))
    {
        if (0 != (cipherMask & pSSLCipherSuiteCtx->ciperSuites))
        {
            for (i=0; i<WDRV_WINC_NUM_CIPHER_SUITES; i++)
            {
                if (cipherMask == _cipherSuiteMap[i].bitMask)
                {
                    *pCipherSuiteList++ = _cipherSuiteMap[i].ianaID;
                    numCipherSuites++;
                }
            }
        }

        cipherMask <<= 1;
    }

    return numCipherSuites;
}

//*******************************************************************************
/*
  Function:
    WDRV_WINC_STATUS WDRV_WINC_SSLActiveCipherSuitesSet
    (
        DRV_HANDLE handle,
        WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx,
        WDRV_WINC_SSL_CIPHERSUITELIST_CALLBACK pfSSLListCallback
    )

  Summary:
    Set the active cipher suites.

  Description:
    Passes the active cipher suite list to the WINC.

  Remarks:
    See wdrv_winc_ssl.h for usage information.

*/

WDRV_WINC_STATUS WDRV_WINC_SSLActiveCipherSuitesSet
(
    DRV_HANDLE handle,
    WDRV_WINC_CIPHER_SUITE_CONTEXT *pSSLCipherSuiteCtx,
    WDRV_WINC_SSL_CIPHERSUITELIST_CALLBACK pfSSLListCallback
)
{
    WDRV_WINC_DCPT *const pDcpt = (WDRV_WINC_DCPT *const )handle;

    /* Ensure the driver handle and user pointer is valid. */
    if ((NULL == pDcpt) || (NULL == pSSLCipherSuiteCtx))
    {
        return WDRV_WINC_STATUS_INVALID_ARG;
    }

    /* Ensure the driver instance has been opened for use. */
    if (false == pDcpt->isOpen)
    {
        return WDRV_WINC_STATUS_NOT_OPEN;
    }

    pDcpt->pfSSLCipherSuiteListCB = pfSSLListCallback;

    /* Set the active cipher suite. */
    if (M2M_SUCCESS != m2m_ssl_set_active_ciphersuites(pSSLCipherSuiteCtx->ciperSuites))
    {
        return WDRV_WINC_STATUS_REQUEST_ERROR;
    }

    return WDRV_WINC_STATUS_OK;
}
