@echo off
SET PATH=..\..\Compiler\WINDOWS\Other Utilities;%PATH%
goto start

:yabause
echo Starting Yabause...
call scripts\run_with_yabause
goto stop

:ssf
echo Starting SSF with DAEMON Tools...
SET CURRENT_DIR=%cd%
SET EMULATOR_DIR=..\..\Emulators
SET DT_DIR=C:\Program Files\DAEMON Tools Ultra

if exist game.iso (
    echo Mounting image...
    "%DT_DIR%\DTCommandLine.exe" --mount --letter "K" --ro --path "%cd%\game.iso"
    cd "%EMULATOR_DIR%\SSF\"
    echo Running SSF...
    "SSF.exe"
    echo Unmounting image...
    "%DT_DIR%\DTCommandLine.exe" --unmount --letter "K"
    cd "%CURRENT_DIR%"
) else (
    echo Please compile first !
    pause
)
goto stop

:kronos
echo Starting Kronos...
call scripts\run_with_kronos
goto stop

:yaba
echo Starting Yaba Sanshiro...
call scripts\run_with_yabaSanshiro
goto stop

:mednafen
echo Starting Mednafen...
call scripts\run_with_mednafen
goto stop

:help
echo run clean
echo   clean environment
echo.
echo run compile [ssf]
echo run compile [yabause]
echo run compile [kronos]
echo run compile [yaba]
echo run compile [mednafen]
echo   clean and compile and optionally run ssf, yabause, kronos, yabaSanshiro or mednafen
echo.
echo run ssf
echo run yabause
echo run kronos
echo run yaba
echo run mednafen
echo   start ssf, yabause, kronos, yabaSanshiro or mednafen
goto stop

:start
if "%1" == "compile" (
    echo Cleaning...
    rm -f ./src/images.h
    rm -f ./cd/ART/*.TGA
    call scripts\clean
    echo Compiling...
    call python genimages.py
    call scripts\compile
)
) else (
    if "%1" == "qcompile" (
        echo Quick Cleaning...
        call scripts\clean
        echo Compiling...
        call scripts\compile
    )
) else (
    if "%1" == "clean" (
        echo Cleaning...
        rm -f ./src/images.h
        rm -f ./cd/ART/*.TGA
        call scripts\clean
    )
)

if "%1" == "yabause" (
    goto yabause
)
) else (
    if "%1" == "ssf" (
        goto ssf
    )
) else (
    if "%1" == "kronos" (
        goto kronos
    )
) else (
    if "%1" == "yaba" (
        goto yaba
    )
) else (
    if "%1" == "mednafen" (
        goto mednafen
    )
) else (
    if "%2" == "yabause" (
        goto yabause
    )
) else (
    if "%2" == "ssf" (
        goto ssf
    )
) else (
    if "%2" == "kronos" (
        goto kronos
    )
) else (
    if "%2" == "yaba" (
        goto yaba
    )
) else (
    if "%2" == "mednafen" (
        goto mednafen
    )
)

if "%1" == "y" (
    goto yabause
    )
) else (
    if "%1" == "s" (
        goto ssf
    )
) else (
    if "%1" == "k" (
        goto kronos
    )
) else (
    if "%1" == "ys" (
        goto yaba
    )
) else (
    if "%1" == "m" (
        goto mednafen
    )
) else (
    if "%2" == "y" (
        goto yabause
    )
) else (
    if "%2" == "s" (
        goto ssf
    )
) else (
    if "%2" == "k" (
        goto kronos
    )
) else (
    if "%2" == "ys" (
        goto yaba
    )
) else (
    if "%2" == "m" (
        goto mednafen
    )
)

if "%1" == "" (
    goto help
) else (
    if "%1" == "help" (
        goto help
    )
)

:stop
