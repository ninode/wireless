/*******************************************************************************
  File Name:
    m2m_ssl.h

  Summary:
    WINC SSL Interface.

  Description:
    WINC SSL Interface.
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

/** @defgroup SSLAPI SSL
    @brief
        Provides a description of the SSL Layer.
    @{
        @defgroup SSLCallbacks Callbacks
        @brief
            Provides detail on the available callbacks for the SSL Layer.

        @defgroup SSLEnums Enumerations and Typedefs
        @brief
            Specifies the enums and Data Structures used by the SSL APIs.

        @defgroup SSLFUNCTIONS Functions
        @brief
            Provides detail on the available APIs for the SSL Layer.
    @}
*/

#ifndef __M2M_SSL_H__
#define __M2M_SSL_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "nm_common.h"
#include "m2m_types.h"
#include "nmdrv.h"
#include "ecc_types.h"
#include "socket.h"

/*!
@ingroup    SSLCallbacks
@typedef    void (*tpfAppSSLCb)(uint8_t u8MsgType, void* pvMsg);
@brief      A callback to get SSL notifications.
@param[in]  u8MsgType
                The type of the message received.
@param[in]  pvMsg
                A structure to provide notification payload.
*/
typedef void (*tpfAppSSLCb)(uint8_t u8MsgType, void *pvMsg);

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FUNCTION PROTOTYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_init(tpfAppSSLCb pfAppSSLCb);
@brief      Initializes the SSL layer.
@param[in]  pfAppSSLCb
                Application SSL callback function.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_init(tpfAppSSLCb pfAppSSLCb);

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_handshake_rsp(tstrEccReqInfo* strECCResp, uint8_t* pu8RspDataBuff, uint16_t u16RspDataSz);
@brief      Sends ECC responses to the WINC.
@param[in]  strECCResp
                ECC Response struct.
@param[in]  pu8RspDataBuff
                Pointer of the response data to be sent.
@param[in]  u16RspDataSz
                Response data size.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_handshake_rsp(tstrEccReqInfo *strECCResp, uint8_t *pu8RspDataBuff, uint16_t u16RspDataSz);

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_send_certs_to_winc(uint8_t* pu8Buffer, uint32_t u32BufferSz);
@brief      Sends certificates to the WINC.
@param[in]  pu8Buffer
                Pointer to the certificates. The buffer format must match the format of @ref tstrTlsSrvSecHdr.
@param[in]  u32BufferSz
                Size of the certificates.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_send_certs_to_winc(uint8_t *pu8Buffer, uint32_t u32BufferSz);

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_retrieve_cert(uint16_t* pu16CurveType, uint8_t* pu8Hash, uint8_t* pu8Sig, tstrECPoint* pu8Key);
@brief      Retrieve the ECDSA certificate to be verified from the WINC.
@param[in]  pu16CurveType
                Pointer to the certificate curve type.
@param[in]  pu8Hash
                Pointer to the certificate hash.
@param[in]  pu8Sig
                Pointer to the certificate signature.
@param[in]  pu8Key
                Pointer to the certificate Key.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_retrieve_cert(uint16_t *pu16CurveType, uint8_t *pu8Hash, uint8_t *pu8Sig, tstrECPoint *pu8Key);

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_retrieve_hash(uint8_t* pu8Hash, uint16_t u16HashSz);
@brief      Retrieve the certificate hash.
@param[in]  pu8Hash
                Pointer to the certificate hash.
@param[in]  u16HashSz
                Hash size.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_retrieve_hash(uint8_t *pu8Hash, uint16_t u16HashSz);

/*!
@ingroup    SSLFUNCTIONS
@fn         void m2m_ssl_stop_processing_certs(void);
@brief      Allow SSL driver to tidy up in case application does not read all available certificates.
@warning    This API must only be called if some certificates are left unread.
@return     None.
*/
void m2m_ssl_stop_processing_certs(void);

/*!
@ingroup    SSLFUNCTIONS
@fn         void m2m_ssl_ecc_process_done(void);
@brief      Allow SSL driver to tidy up after application has finished processing ECC message.
@warning    This API must be called after receiving a SSL callback with message type @ref M2M_SSL_REQ_ECC.
@return     None.
*/
void m2m_ssl_ecc_process_done(void);

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_set_active_ciphersuites(uint32_t u32SslCsBMP);
@brief      Sets the active ciphersuites.
@details    Override the default Active SSL ciphers in the SSL module with a certain combination selected by
            the caller in the form of a bitmap containing the required ciphers to be on.\n
            There is no need to call this function if the application will not change the default ciphersuites.
@param[in]  u32SslCsBMP
                Bitmap containing the desired ciphers to be enabled for the SSL module. The ciphersuites are defined in
                @ref SSLCipherSuiteID.
                The default ciphersuites are all ciphersuites supported by the firmware with the exception of ECC ciphersuites.
                The caller can override the default with any desired combination.
                If u32SslCsBMP does not contain any ciphersuites supported by firmware, then the current active list will not
                change.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_set_active_ciphersuites(uint32_t u32SslCsBMP);

/*!
@ingroup    SSLFUNCTIONS
@fn         int8_t m2m_ssl_set_cert_verification_mode(tenuCertVerifMode enuMode);
@brief      Sets the TLS certificate verification mode.
@details    By default, the expiry date is checked when processing TLS certificates from the peer.
                (Expiry date must be later than current system time, and current system time must be known.)
            This API allows this check to be weakened or bypassed altogether.
            It is recommended that the check is only weakened temporarily and for well-understood reasons. Once the
            weakening is no longer required then this API should be called again to revert to the default setting.
            The available settings are described in @ref tenuCertVerifMode. The default setting is @ref TLS_CERT_NO_BYPASS.
            One possible usage would be to allow WPA(2) Enterprise connection when the system time is not known.
@see        tenuCertVerifMode
@param[in]  enuMode
                The required verification mode.
@return     The function returns @ref M2M_SUCCESS for success and a negative value otherwise.
*/
int8_t m2m_ssl_set_cert_verification_mode(tenuCertVerifMode enuMode);

#endif /* __M2M_SSL_H__ */