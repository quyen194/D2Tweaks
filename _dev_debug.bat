
@ECHO OFF

SET BUILD_PROFILE=Debug
SET WORKING_DIR=%~dp0
CALL %WORKING_DIR%config.bat

IF NOT EXIST "%GAME_PATH%" (
	ECHO Game Dir Not Exists %GAME_PATH%
	PAUSE
	EXIT
)

SET PATH_D2TWEAKS=%GAME_PATH%\d2tweaks
IF NOT EXIST "%PATH_D2TWEAKS%" (
	MKDIR "%PATH_D2TWEAKS%"
)
XCOPY "%WORKING_DIR%build\!resources\d2tweaks\*" "%GAME_PATH%\d2tweaks\" /E /H /Y
COPY /Y "%WORKING_DIR%build\!resources\%D2TWEAKS_INI%" "%GAME_PATH%\d2tweaks.ini"
COPY /Y "%WORKING_DIR%build\%BUILD_PROFILE%\D2tweaks.dll" "%GAME_PATH%\D2tweaks.dll"
COPY /Y "%WORKING_DIR%build\%BUILD_PROFILE%\D2tweaks.pdb" "%GAME_PATH%\D2tweaks.pdb"
COPY /Y "%WORKING_DIR%build\!resources\%D2TWEAKS_DEBUGGER_INI%" "%GAME_PATH%\%D2TWEAKS_DEBUGGER_INI%"
