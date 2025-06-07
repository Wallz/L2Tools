CALL "%VS140COMNTOOLS%vsvars32.bat"

CALL :GeneratePackage Core
CALL :GeneratePackage Engine
CALL :GeneratePackage ALAudio
CALL :GeneratePackage D3DDrv
CALL :GeneratePackage Fire
CALL :GeneratePackage IpDrv
CALL :GeneratePackage Window
CALL :GeneratePackage WinDrv

pause
goto :eof

:GeneratePackage
SETLOCAL EnableDelayedExpansion
SET _var1=%1

del "!_var1!.txt"
del "!_var1!.def"
del "!_var1!.exp"
del "!_var1!.lib"

dumpbin /EXPORTS "!_var1!.dll" > "!_var1!.txt"
java -cp GenerateDefFromDumpbin.jar Main "!_var1!"
lib /def:"!_var1!.def" /OUT:"!_var1!.lib"

del "../!_var1!/Debug/!_var1!.exp"
copy "!_var1!.exp" "../!_var1!/Debug/!_var1!.exp"
del "../!_var1!/Debug/!_var1!.lib"
copy "!_var1!.lib" "../!_var1!/Debug/!_var1!.lib"

del "../!_var1!/Debug/!_var1!.exp"
move "!_var1!.exp" "../!_var1!/Release/!_var1!.exp"
del "../!_var1!/Release/!_var1!.lib"
move "!_var1!.lib" "../!_var1!/Release/!_var1!.lib"

ENDLOCAL