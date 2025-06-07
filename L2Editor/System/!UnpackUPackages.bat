SET ResultDir=ExportedUPackages

FOR %%f IN (*.u) DO (CALL :UnpackUPackage %%f)

pause
goto :eof

:UnpackUPackage
SETLOCAL EnableDelayedExpansion
SET _var1=%1

rmdir /S /Q "!ResultDir!/!_var1!/Classes"
mkdir "!ResultDir!/!_var1!/Classes"
rmdir /S /Q "!ResultDir!/!_var1!/Inc"
mkdir "!ResultDir!/!_var1!/Inc"
rmdir /S /Q "!ResultDir!/!_var1!/Textures"
mkdir "!ResultDir!/!_var1!/Textures"

ucc batchexport %1 class uc "!ResultDir!\!_var1!\Classes"
ucc batchexport %1 class h "!ResultDir!\!_var1!\Inc"
ucc batchexport %1 texture tga "!ResultDir!\!_var1!\Textures"
ucc batchexport %1 texture pcx "!ResultDir!\!_var1!\Textures"

ENDLOCAL