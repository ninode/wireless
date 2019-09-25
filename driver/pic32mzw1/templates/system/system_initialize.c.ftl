    /* MAC Driver Initialization */
    IPC21bits.RFMACIP = 1;
    IPC21bits.RFMACIS = 0;

    IPC21bits.RFTM0IP = 1;
    IPC21bits.RFTM0IS = 1;

    /* Initialize the PIC32MZW1 Driver */
    sysObj.drvWifiPIC32MZW1 = WDRV_PIC32MZW_Initialize(WDRV_PIC32MZW_SYS_IDX_0, NULL);