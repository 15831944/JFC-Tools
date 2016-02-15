@ECHO OFF
setlocal ENABLEEXTENSIONS
setlocal EnableDelayedExpansion

@ECHO Searching for Visual Studio...
REM find visual studio VC installation path
SET KEY_NAME="HKLM\SOFTWARE\Microsoft\VisualStudio\11.0\Setup\VC"
SET VALUE_NAME=ProductDir
FOR /F "usebackq tokens=1-2*" %%a IN (`REG QUERY !KEY_NAME! /v !VALUE_NAME! 2^>nul`) DO (
    SET VisualStudioVC="%%c"
)
IF NOT DEFINED VisualStudioVC (
	SET KEY_NAME_="HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\11.0\Setup\VC"
	SET VALUE_NAME_=ProductDir
	FOR /F "usebackq tokens=1-2*" %%a IN (`REG QUERY !KEY_NAME_! /v !VALUE_NAME_! 2^>nul`) DO (
		SET VisualStudioVC="%%c"
	)

	IF NOT DEFINED VisualStudioVC (
		ECHO visual studio not found !
		PAUSE
		EXIT /b 1
	)
)

REM installing visual studio VC environment variables
call !VisualStudioVC!\vcvarsall.bat x86_amd64

xsd.exe IMP_CODI_PRESS_TARIFSyyyyMMddhhmmsslog.xml
xsd.exe /c /l:CS /eld /edb /n:IMP_CODI_PRESS_TARIFS_LOG IMP_CODI_PRESS_TARIFSyyyyMMddhhmmsslog.xsd

PAUSE
EXIT /b 0

