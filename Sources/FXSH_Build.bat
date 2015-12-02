@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist SOUND.G1A  del SOUND.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\Casio_SDK\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\Casio_SDK\Tools\MakeAddinHeader363.exe" "C:\Users\mosnier\Desktop\Sound4Calc\Sources"
if not exist SOUND.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

