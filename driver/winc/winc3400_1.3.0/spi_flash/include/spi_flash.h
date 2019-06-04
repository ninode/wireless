/*******************************************************************************
  WINC3400 SPI Flash Interface

  File Name:
    spi_flash.h

  Summary:
    WINC3400 SPI Flash Interface

  Description:
    WINC3400 SPI Flash Interface
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc. All rights reserved.

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

/** @defgroup SPIFLASHAPI SPI FLASH
 */

#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include "nm_common.h"
#include "nmbus.h"
#include "spi_flash_map.h"

/** @defgroup SPIFLASHFUNCTIONS Functions
 *  @ingroup SPIFLASHAPI
 */

/**@{*/
/*!
 * @fn           uint32_t spi_flash_get_size(void);
 * @brief        Returns with \ref uint32_t value which is total flash size\n
 * @note         Returned value in Mb (Mega Bit).
 * @return       SPI flash size in case of success and a ZERO value in case of failure.
 */
uint32_t spi_flash_get_size(void);

/*!
 * @fn             int8_t spi_flash_read(uint8_t *, uint32_t, uint32_t);
 * @brief          Read a specified portion of data from SPI Flash.\n
 * @param [out]    pu8Buf
 *                 Pointer to data buffer which will be filled with data in case of successful operation.
 * @param [in]     u32Addr
 *                 Address (Offset) to read from at the SPI flash.
 * @param [in]     u32Sz
 *                 Total size of data to be read in bytes
 * @warning
 *                 - Address (offset) plus size of data must not exceed flash size.\n
 *                 - No firmware is required for reading from SPI flash.\n
 *                 - In case of there is a running firmware, it is required to pause your firmware first
 *                   before any trial to access SPI flash to avoid any racing between host and running firmware on bus using
 *                   @ref m2m_wifi_download_mode
 * @note
 *                 - It is blocking function\n
 * @sa             m2m_wifi_download_mode, spi_flash_get_size
 * @return        The function returns @ref M2M_SUCCESS for successful operations  and a negative value otherwise.
 */
int8_t spi_flash_read(uint8_t *pu8Buf, uint32_t u32Addr, uint32_t u32Sz);

/*!
 * @fn             int8_t spi_flash_write(uint8_t *, uint32_t, uint32_t);
 * @brief          Write a specified portion of data to SPI Flash.\n
 * @param [in]     pu8Buf
 *                 Pointer to data buffer which contains the data to be written.
 * @param [in]     u32Offset
 *                 Address (Offset) to write at the SPI flash.
 * @param [in]     u32Sz
 *                 Total number of size of data bytes
 * @note
 *                 - It is blocking function\n
 *                 - It is user's responsibility to verify that data has been written successfully
 *                   by reading data again and comparing it with the original.
 * @warning
 *                 - Address (offset) plus size of data must not exceed flash size.\n
 *                 - No firmware is required for writing to SPI flash.\n
 *                 - In case of there is a running firmware, it is required to pause your firmware first
 *                   before any trial to access SPI flash to avoid any racing between host and running firmware on bus using
 *                   @ref m2m_wifi_download_mode.
 *                 - Before writing to any section, it is required to erase that section first.
 * @sa             m2m_wifi_download_mode, spi_flash_get_size, spi_flash_erase
 * @return       The function returns @ref M2M_SUCCESS for successful operations  and a negative value otherwise.
 */
int8_t spi_flash_write(uint8_t* pu8Buf, uint32_t u32Offset, uint32_t u32Sz);

/*!
 * @fn             int8_t spi_flash_erase(uint32_t, uint32_t);
 * @brief          Erase a specified portion of SPI Flash.\n
 * @param [in]     u32Offset
 *                 Address (Offset) to erase from the SPI flash.
 * @param [in]     u32Sz
 *                 Total number of bytes required to be erased.
 * @note         It is blocking function \n
 * @warning
 *                 - Address (offset) plus size of data must not exceed flash size.\n
 *                 - No firmware is required for writing to SPI flash.\n
 *                 - In case of there is a running firmware, it is required to pause your firmware first
 *                   before any trial to access SPI flash to avoid any racing between host and running firmware on bus using
 *                   @ref m2m_wifi_download_mode
 * @sa             m2m_wifi_download_mode, spi_flash_get_size
 * @return       The function returns @ref M2M_SUCCESS for successful operations  and a negative value otherwise.
 *  \section SPIFLASHExample Example
 * @code{.c}
 *                  #include "spi_flash.h"
 *
 *                  #define DATA_TO_REPLACE "THIS IS A NEW SECTOR IN FLASH"
 *
 *                  int main()
 *                  {
 *                      uint8_t   au8FlashContent[FLASH_SECTOR_SZ] = {0};
 *                      uint32_t  u32FlashTotalSize = 0;
 *                      uint32_t  u32FlashOffset = 0;
 *
 *                      ret = m2m_wifi_download_mode();
 *                      if(M2M_SUCCESS != ret)
 *                      {
 *                          printf("Unable to enter download mode\r\n");
 *                      }
 *                      else
 *                      {
 *                          u32FlashTotalSize = spi_flash_get_size();
 *                      }
 *
 *                      while((u32FlashTotalSize > u32FlashOffset) && (M2M_SUCCESS == ret))
 *                      {
 *                          ret = spi_flash_read(au8FlashContent, u32FlashOffset, FLASH_SECTOR_SZ);
 *                          if(M2M_SUCCESS != ret)
 *                          {
 *                              printf("Unable to read SPI sector\r\n");
 *                              break;
 *                          }
 *                          memcpy(au8FlashContent, DATA_TO_REPLACE, strlen(DATA_TO_REPLACE));
 *
 *                          ret = spi_flash_erase(u32FlashOffset, FLASH_SECTOR_SZ);
 *                          if(M2M_SUCCESS != ret)
 *                          {
 *                              printf("Unable to erase SPI sector\r\n");
 *                              break;
 *                          }
 *
 *                          ret = spi_flash_write(au8FlashContent, u32FlashOffset, FLASH_SECTOR_SZ);
 *                          if(M2M_SUCCESS != ret)
 *                          {
 *                              printf("Unable to write SPI sector\r\n");
 *                              break;
 *                          }
 *                          u32FlashOffset += FLASH_SECTOR_SZ;
 *                      }
 *
 *                      if(M2M_SUCCESS == ret)
 *                      {
 *                          printf("Successful operations\r\n");
 *                      }
 *                      else
 *                      {
 *                          printf("Failed operations\r\n");
 *                      }
 *
 *                      while(1);
 *                      return M2M_SUCCESS;
 *                  }
 * @endcode
 */

int8_t spi_flash_erase(uint32_t u32Offset, uint32_t u32Sz);

/**@}
 */

#endif  //__SPI_FLASH_H__