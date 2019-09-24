######################  WiFi Driver  ######################
def loadModule():
    print("Load Module: Harmony WiFi Driver")

    ## WIFI Driver
    drvWifiWincComponent = Module.CreateComponent("drvWifiWinc", "WINC", "/Harmony/Drivers/Wi-Fi/", "driver/winc/config/drv_winc.py")
    drvWifiWincComponent.addCapability("wdrv_winc", "WDRV_WINC")
    
    drvWifiPic32mzw1Component = Module.CreateComponent("drvWifiPic32mzw1", "PIC32MZW1", "/Harmony/Drivers/Wi-Fi/", "driver/pic32mzw1/config/drv_pic32mzw1.py")
    drvWifiPic32mzw1Component.addCapability("wdrv_pic32mzw1", "WDRV_PIC32MZW1")
    drvWifiPic32mzw1Component.addCapability("libdrvPic32mzw1Mac", "MAC")

    ## Serial Bridge Application
    appSerBridge = Module.CreateComponent("appWincSerialBridge", "Serial Bridge Application", "/Applications/WINC/", "app/serial_bridge/config/app_winc_serial_bridge.py")
