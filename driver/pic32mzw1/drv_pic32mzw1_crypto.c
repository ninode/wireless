/*******************************************************************************
  PIC32MZW Wireless Driver

  File Name:
    wdrv_pic32mzw_crypto.c

  Summary:
    PIC32MZW wireless driver interface to crypto functionality.

  Description:
    PIC32MZW wireless driver interface to crypto functionality.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

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
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "drv_pic32mzw1.h"
#include "drv_pic32mzw1_crypto.h"
#include "crypto/crypto.h"

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Driver Defines
// *****************************************************************************
// *****************************************************************************

static union {
    CRYPT_HMAC_CTX   hmac_context;
    CRYPT_RNG_CTX    rng_context;
} g_context;

/*****************************************************************************/
/* Big integer functions:   DRV_PIC32MZW1_Crypto_BigIntCompare               */
/*                          DRV_PIC32MZW1_Crypto_BigIntModRandom             */
/*                                                                           */
/* Arrays pointed to by params are big endian.                               */
/*****************************************************************************/
/* 1 if check > baseline, -1 if check < baseline, 0 if check == baseline. */
int DRV_PIC32MZW1_Crypto_BigIntCompare
(
        const uint8_t   *check,
        const uint8_t   *baseline,
        uint16_t        param_len
)
{
    if ((NULL == check) || (NULL == baseline))
    {
        /* Invalid parameters, return value is meaningless. */
        return 0;
    }
    while (param_len--)
    {
        if (*check > *baseline)
        {
            return 1;
        }
        if (*check < *baseline)
        {
            return -1;
        }
        check++;
        baseline++;
    }
    return 0;
}
/* out = random number less than limit (if limit != NULL).                   */
/* out is only valid if return is true.                                      */
bool DRV_PIC32MZW1_Crypto_BigIntRandom
(
        const uint8_t   *limit,
        uint8_t         *out,
        uint16_t        param_len
)
{
    if (NULL == out)
    {
        return false;
    }
    if (0 > CRYPT_RNG_Initialize(&g_context.rng_context))
    {
        return false;
    }

    if (NULL == limit)
    {
        /* If limit is not specified, any random block will do. */
        if (0 > CRYPT_RNG_BlockGenerate(&g_context.rng_context,
                                        out,
                                        param_len))
        {
            return false;
        }
    }
    else
    {
        /* If the MSB of the limit is 0, set the MSB of the output to 0. */
        while ((param_len > 0) && (0 == *limit))
        {
            *out++ = 0;
            limit++;
            param_len--;
        }
        if (0 == param_len)
        {
            return false;
        }
        /* Generate a random block of length param_len, and loop until it is */
        /* less than limit.                                                  */
        do
        {
            if (0 > CRYPT_RNG_BlockGenerate(&g_context.rng_context,
                                            out,
                                            param_len))
            {
                return false;
            }
        } while (-1 != DRV_PIC32MZW1_Crypto_BigIntCompare(out, limit, param_len));
    }
    return true;
}

/*****************************************************************************/
/* HMAC_SHA256 functions.   DRV_PIC32MZW1_Crypto_HMACSHA256                  */
/*****************************************************************************/
/* Run a HMACSHA256 operation. */
bool DRV_PIC32MZW1_Crypto_HMACSHA256
(
        const uint8_t   *salt,
        uint16_t        salt_len,
        const buffer_t  *input_data_buffers,
        int             num_buffers,
        uint8_t         *digest
)
{
    int i;

    if ((NULL == salt) || (NULL == input_data_buffers) || (NULL == digest))
    {
        return false;
    }
    for (i = 0; i < num_buffers; i++)
    {
        if (NULL == input_data_buffers[i].data)
        {
            return false;
        }
    }

    /* Initialise the HMAC. */
    CRYPT_HMAC_SetKey(&g_context.hmac_context, CRYPT_HMAC_SHA256, salt, salt_len);

    /* Add each buffer. */
    for (i = 0; i < num_buffers; i++)
    {
        CRYPT_HMAC_DataAdd( &g_context.hmac_context,
                            input_data_buffers[i].data,
                            input_data_buffers[i].data_len);
    }

    /* Finalise the HMAC and obtain the digest. */
    CRYPT_HMAC_Finalize(&g_context.hmac_context, digest);

    return true;
}
