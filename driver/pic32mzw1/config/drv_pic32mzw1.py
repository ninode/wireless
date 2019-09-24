###############################################################################
###################### PIC32MZW1 Drvier Configurations ########################
###############################################################################

def importIncFile(component, configName, incFileEntry):
    incFilePath  = incFileEntry[0]
    isEnabled    = incFileEntry[1][0]
    callback     = incFileEntry[1][1]
    dependencies = incFileEntry[1][2]

    incFilePathTup = incFilePath.split('/')

    if len(incFilePathTup) == 1:
        secName = ''
        incFile = incFilePathTup[0]
    else:
        secName = incFilePathTup[0]
        incFile = incFilePathTup[1]

    symName = incFile.replace('.', '_').upper()
    secSName = secName + '/'
    secDName = secSName

    incFileSym = component.createFileSymbol(symName, None)
    incFileSym.setSourcePath('driver/pic32mzw1/' + secSName + 'include/' + incFile)
    incFileSym.setOutputName(incFile)
    incFileSym.setDestPath('driver/wifi/pic32mzw1/include/' + secDName)
    incFileSym.setProjectPath('config/' + configName + '/driver/wifi/pic32mzw1/include/' + secDName)
    incFileSym.setType('HEADER')
    incFileSym.setOverwrite(True)
    incFileSym.setEnabled(isEnabled)

    if callback and dependencies:
        incFileSym.setDependencies(callback, dependencies)

def importSrcFile(component, configName, srcFileEntry):
    srcFilePath  = srcFileEntry[0]
    isEnabled    = srcFileEntry[1][0]
    callback     = srcFileEntry[1][1]
    dependencies = srcFileEntry[1][2]

    srcFilePathTup = srcFilePath.rsplit('/', 1)

    if len(srcFilePathTup) == 1:
        secName = ''
        srcFile = srcFilePathTup[0]
    else:
        secName = srcFilePathTup[0]
        srcFile = srcFilePathTup[1]

    srcFilePrefix   = ''
    symName = srcFile.replace('.', '_').upper()
    secSName = secName + '/'
    secDName = secSName

    srcFileSym = component.createFileSymbol(symName, None)
    srcFileSym.setSourcePath('driver/pic32mzw1/' + secSName + srcFile)
    srcFileSym.setOutputName(srcFile.rsplit('/', 1)[-1])
    srcFileSym.setDestPath('driver/wifi/pic32mzw1/' + secDName)
    srcFileSym.setProjectPath('config/' + configName + '/driver/wifi/pic32mzw1/' + secDName)
    srcFileSym.setType('SOURCE')
    srcFileSym.setEnabled(isEnabled)

    if callback and dependencies:
        srcFileSym.setDependencies(callback, dependencies)

def setIncPath(component, configName, incPathEntry):
    incPath      = incPathEntry[0]
    isEnabled    = incPathEntry[1][0]
    callback     = incPathEntry[1][1]
    dependencies = incPathEntry[1][2]

    incPathSym = component.createSettingSymbol('PIC32MZW1_INC_PATH' + incPath.replace('.', '_').replace('/', '_').upper(), None)
    incPathSym.setValue('../src/config/' + configName + '/driver/wifi/pic32mzw1/include' + incPath + ';')
    incPathSym.setCategory('C32')
    incPathSym.setKey('extra-include-directories')
    incPathSym.setAppend(True, ';')
    incPathSym.setEnabled(isEnabled)
    incPathSym.setDependencies(callback, dependencies)

def instantiateComponent(drvPic32mzw1Component):
    print('PIC32MZW1 Driver Component')
    configName = Variables.get('__CONFIGURATION_NAME')

    ############################################################################
    #### Code Generation ####
    ############################################################################

    condAlways = [True, None, []]

    wdrvIncFiles = [
        ['drv_pic32mzw1.h',             condAlways],
        ['wdrv_pic32mzw.h',             condAlways],
        ['wdrv_pic32mzw_api.h',         condAlways],
        ['wdrv_pic32mzw_assoc.h',       condAlways],
        ['wdrv_pic32mzw_authctx.h',     condAlways],
        ['wdrv_pic32mzw_bssctx.h',      condAlways],
        ['wdrv_pic32mzw_bssfind.h',     condAlways],
        ['wdrv_pic32mzw_cfg.h',         condAlways],
        ['wdrv_pic32mzw_client_api.h',  condAlways],
        ['wdrv_pic32mzw_common.h',      condAlways],
        ['wdrv_pic32mzw_debug.h',       condAlways],
        ['wdrv_pic32mzw_mac.h',         condAlways],
        ['wdrv_pic32mzw_softap.h',      condAlways],
        ['wdrv_pic32mzw_sta.h',         condAlways]
    ]

    for incFileEntry in wdrvIncFiles:
        importIncFile(drvPic32mzw1Component, configName, incFileEntry)

    wdrvSrcFiles = [
        ['wdrv_pic32mzw.c',             condAlways],
        ['wdrv_pic32mzw_assoc.c',       condAlways],
        ['wdrv_pic32mzw_authctx.c',     condAlways],
        ['wdrv_pic32mzw_bssctx.c',      condAlways],
        ['wdrv_pic32mzw_bssfind.c',     condAlways],
        ['wdrv_pic32mzw_cfg.c',         condAlways],
        ['wdrv_pic32mzw_int.c',         condAlways],
        ['wdrv_pic32mzw_softap.c',      condAlways],
        ['wdrv_pic32mzw_sta.c',         condAlways]
    ]

    for srcFileEntry in wdrvSrcFiles:
        importSrcFile(drvPic32mzw1Component, configName, srcFileEntry)

    wdrvIncPaths = [
        ['/', condAlways]
    ]

    for incPathEntry in wdrvIncPaths:
        setIncPath(drvPic32mzw1Component, configName, incPathEntry)

	pic32mzw1_a = drvPic32mzw1Component.createLibrarySymbol(None, None)
	pic32mzw1_a.setDestPath('driver/wifi/pic32mzw1/lib')
	pic32mzw1_a.setOutputName('pic32mzw1.a')
	pic32mzw1_a.setSourcePath('driver\pic32mzw1/pic32mzw1.a')
