/*** Wi-Fi Driver Configuration ***/
<#if DRV_WIFI_USE_DRIVER>
<#if WIFI_EXTERNAL_INTERRUPT_USE == "External_Interrupt">
	#define WDRV_WINC_INT_SOURCE 	EIC_IRQn
	#error "Please select an external interrupt instance for Wi-Fi driver."
<#elseif WIFI_EXTERNAL_INTERRUPT_USE == "Change_Notification">
	#define WDRV_WINC_INT_SOURCE INT_SOURCE_CHANGE_NOTICE
	#define WDRV_WINC_INT_SOURCE_CN_PORT WDRV_WINC_INT_PORT
</#if>
	#define WDRV_WINC_SPI_INDEX 	${WIFI_DRIVER_SPI_INSTANCE_INX}
<#if WIFI_DRIVER_MODE_CHOICE == "Ethernet_Mode">
	#define WDRV_WINC_NETWORK_MODE_ETHERNET
<#else>
	#define WDRV_WINC_NETWORK_MODE_SOCKET
</#if>
<#if WIFI_DEVICE_SELECT == "WINC1500">
	#define WDRV_WINC_DEVICE_WINC1500
</#if>
	#define WIFI_DRV_VERSION 	${WIFI_DRV_VERSION}
<#if WIFI_DRV_VERSION == "19.5.4">
<#elseif WIFI_DRV_VERSION == "19.6.1">
	#define WDRV_WINC_DEVICE_SPLIT_INIT
	#define WDRV_WINC_DEVICE_ENTERPRISE_CONNECT
	#define WDRV_WINC_DEVICE_EXT_CONNECT_PARAMS
	#define WDRV_WINC_DEVICE_BSS_ROAMING
	#define WDRV_WINC_DEVICE_FLEXIBLE_FLASH_MAP
</#if>
<#if WIFI_DRIVER_MODE_CHOICE == "Socket_Mode">
	#define WDRV_WINC_DEVICE_CONF_NTP_SERVER
	#define WDRV_WINC_DEVICE_HOST_FILE_DOWNLOAD
	#define WDRV_WINC_DEVICE_SOFT_AP_EXT
<#else>
	#define WDRV_WINC_DEVICE_DYNAMIC_BYPASS_MODE
</#if>
<#if WIFI_DEVICE_SELECT == "WINC3400">
	#define WDRV_WINC_DEVICE_WINC3400
	#define WDRV_WINC_DEVICE_SPLIT_INIT
	#define WDRV_WINC_DEVICE_MULTI_GAIN_TABLE
</#if>
<#if WIFI_DRIVER_MODE_CHOICE == "Socket_Mode">
	#define WDRV_WINC_DEVICE_CONF_NTP_SERVER
	#define WDRV_WINC_DEVICE_SOFT_AP_EXT
<#else>
	#define WDRV_WINC_DEVICE_DYNAMIC_BYPASS_MODE
</#if>
<#if WIFI_DRIVER_LOG_LEVEL == "None">
	#define WDRV_WINC_DEBUG_LEVEL   WDRV_WINC_DEBUG_TYPE_NONE
<#elseif WIFI_DRIVER_LOG_LEVEL == "Error">
	#define WDRV_WINC_DEBUG_LEVEL   WDRV_WINC_DEBUG_TYPE_ERROR
<#elseif WIFI_DRIVER_LOG_LEVEL == "Inform">
	#define WDRV_WINC_DEBUG_LEVEL   WDRV_WINC_DEBUG_TYPE_INFORM
<#elseif WIFI_DRIVER_LOG_LEVEL == "Trace">
	#define WDRV_WINC_DEBUG_LEVEL   WDRV_WINC_DEBUG_TYPE_TRACE
<#elseif WIFI_DRIVER_LOG_LEVEL == "Verbose">
	#define WDRV_WINC_DEBUG_LEVEL   WDRV_WINC_DEBUG_TYPE_VERBOSE
</#if>
</#if>


