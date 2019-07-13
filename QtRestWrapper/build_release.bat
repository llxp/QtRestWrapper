@echo off
setlocal enabledelayedexpansion

SET buildcommand=%1

REM Defines region
REM ----------------------------------------------------------------

SET mypath=%~dp0
SET binpath=%mypath%bin
SET objpath=%mypath%obj
SET includepath=%mypath%include
SET mocpath=%mypath%moc
SET srcpath=%mypath%src
SET qtpath=C:\Qt\5.13.0\msvc2017_64

SET projectname=QtRestWrapper
SET qtlibpath=%qtpath%\lib
SET qtincludepath=%qtpath%\include
SET pdbfilepath=%objpath%\%projectname%.vc.pdb
SET outputpath=%binpath%\%projectname%.dll

SET qtmodules=QtCore;QtGui;QtWidgets;QtNetwork;QtANGLE;Qtnetwork;QtWebChannel;QtWebEngine;QtWebEngineCore;QtWebEngineWidgets
SET compilerflags=/MP /c /nologo /Zc:wchar_t /FS /Zc:rvalueCast /Zc:inline /Zc:strictStrings /Zc:throwingNew /Zc:referenceBinding /Zc:__cplusplus /Os /MD /W3 /w34100 /w34189 /w44996 /w44456 /w44457 /w44458 /wd4577 /wd4467 /EHsc /DUNICODE /D_UNICODE /DWIN32 /D_ENABLE_EXTENDED_ALIGNED_STORAGE /DWIN64 /DQTRESTWRAPPER_LIBRARY /DQT_DEPRECATED_WARNINGS /DQT_NO_DEBUG /DQT_WEBENGINEWIDGETS_LIB /DQT_PRINTSUPPORT_LIB /DQT_WIDGETS_LIB /DQT_WEBENGINECORE_LIB /DQT_QUICK_LIB /DQT_GUI_LIB /DQT_WEBCHANNEL_LIB /DQT_QML_LIB /DQT_NETWORK_LIB /DQT_POSITIONING_LIB /DQT_CORE_LIB /DNDEBUG /D_WINDLL
SET linkerflags=/NOLOGO /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /DLL /SUBSYSTEM:WINDOWS /MANIFEST:embed
SET mocflags=-DUNICODE -D_UNICODE -DWIN32 -D_ENABLE_EXTENDED_ALIGNED_STORAGE -DWIN64 -DQTRESTWRAPPER_LIBRARY -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_WEBENGINEWIDGETS_LIB -DQT_PRINTSUPPORT_LIB -DQT_WIDGETS_LIB -DQT_WEBENGINECORE_LIB -DQT_QUICK_LIB -DQT_GUI_LIB -DQT_WEBCHANNEL_LIB -DQT_QML_LIB -DQT_NETWORK_LIB -DQT_POSITIONING_LIB -DQT_CORE_LIB -DNDEBUG -D_WINDLL --compiler-flavor=msvc

REM ----------------------------------------------------------------


IF [%buildcommand%] == [] GOTO build
IF %buildcommand% == build GOTO build
IF %buildcommand% == clean GOTO clean

ECHO Wrong parameter
GOTO:eof

:build

IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
) else (
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
)



SET compilerflags=!compilerflags! /Fd%pdbfilepath%
SET linkerflags=!linkerflags! /OUT:%outputpath%

SET includelist=%includepath%
SET includelist=%includelist%;%includepath%\private
SET includelist=%includelist%;%qtincludepath%
SET includelist=%includelist%;%qtpath%\mkspecs\win32-msvc
SET includelist=%includelist%;%mocpath%
SET includelist=%includelist%;%mypath%

FOR %%i in (%qtmodules%) DO (
    SET qtmodule=%%i
    SET includelist=!includelist!;%qtincludepath%\!qtmodule!
)

FOR %%f in (%qtlibpath%\*.lib) DO (
    SET linkerfiles=!linkerfiles! %%f
)

ECHO variables initialized!

FOR %%n in (%includelist%) DO (
    SET localvar=%%n
    SET compilerflags=!compilerflags! /I!localvar!
    SET mocflags=!mocflags! -I!localvar!
)

mkdir %mocpath%
mkdir %objpath%

ECHO directories created!

PUSHD %includepath%
ECHO Generating moc files
FOR /r %%i in (*.h) DO (
    %qtpath%\bin\moc.exe %mocflags% %%i -o ..\moc\%%~ni_moc.cpp
)
POPD
PUSHD %mocpath%
FOR %%i in (*) DO (
    if %%~zi == 0 del "%%i"
)
POPD

ECHO Compiling cpp files
PUSHD %srcpath%
FOR /r %%i in (*.cpp) DO (
    set sourcefile=%%i
    cl.exe !compilerflags! -Fo..\obj\ !sourcefile!
)
POPD
PUSHD %mocpath%
FOR /r %%i in (*.cpp) DO (
    set sourcefile=%%i
    cl.exe !compilerflags! -Fo..\obj\  !sourcefile!
)
POPD

ECHO !linkerfiles! > %objpath%\linkerfile.txt
ECHO Linking object files
PUSHD %objpath%
FOR %%l in (*.obj) DO (
    SET objfile=%%l
    ECHO obj\!objfile! >> %objpath%\linkerfile.txt
)
POPD

PUSHD %mypath%
link.exe !linkerflags! @%objpath%\linkerfile.txt
POPD

:CLEAN
IF EXIST %objpath% (
    IF EXIST %objpath%\*.ilk del %objpath%\*.ilk
    IF EXIST %objpath%\*.obj del %objpath%\*.obj
    IF EXIST %objpath%\*.pdb del %objpath%\*.pdb
    IF EXIST %objpath%\*.txt del %objpath%\*.txt
    rmdir %objpath%
)

IF EXIST %mocpath% (
    IF EXIST %mocpath%\*.cpp del %mocpath%\*.cpp
    rmdir %mocpath%
)

IF EXIST %binpath% (
    IF EXIST %binpath%\*.ilk del bin\*.ilk
)

ECHO Cleared all compiler generated temporary files

@echo on