void __ISR(_RFMAC_VECTOR, ipl1AUTO0) _IntHandlerRfMacInstance0(void)
{
    WDRV_PIC32MZW_TasksRFMACISR(sysObj.drvWifiPIC32MZW1);
}

void __ISR(_RFTM0_VECTOR, ipl1AUTO0) _IntHandlerRfTimer0Instance0(void)
{
    WDRV_PIC32MZW_TasksRFTimer0ISR(sysObj.drvWifiPIC32MZW1);
}
