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

#ifndef _DRV_PIC32MZW1_CRYPTO_H
#define _DRV_PIC32MZW1_CRYPTO_H

#include <stdint.h>
#include <stdbool.h>

#define DRV_PIC32MZW1_CRYPTO_SHA256_DIGEST_SZ   32

/* Generic type for handling data buffers. */
typedef struct
{
    const uint8_t   *data;
    uint16_t        data_len;
} buffer_t;

/*****************************************************************************/
/* Big integer functions:   DRV_PIC32MZW1_Crypto_BigIntCompare               */
/*                          DRV_PIC32MZW1_Crypto_BigIntRandom                */
/*                                                                           */
/* Arrays pointed to by params are big endian.                               */
/*****************************************************************************/
/* 1 if check > baseline, -1 if check < baseline, 0 if check == baseline. */
int DRV_PIC32MZW1_Crypto_BigIntCompare
(
        const uint8_t   *check,
        const uint8_t   *baseline,
        uint16_t        param_len
);
/* out = random number less than limit (if limit != NULL).                   */
/* out is only valid if return is true.                                      */
bool DRV_PIC32MZW1_Crypto_BigIntRandom
(
        const uint8_t   *limit,
        uint8_t         *out,
        uint16_t        param_len
);

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
);

#endif /* _DRV_PIC32MZW1_CRYPTO_H */