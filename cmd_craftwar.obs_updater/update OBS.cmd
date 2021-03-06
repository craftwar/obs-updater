@echo off
cd /d "%~dp0"

if NOT DEFINED file_url (
	call "internal or fewer usages\check_cpu.cmd"
	if DEFINED favor_arch (
		set file_url=https://github.com/craftwar/obs-studio/releases/download/git/OBS-git-craftwar-%favor_arch%.7z
	)
	rem echo %favor_arch%
)
if NOT DEFINED file (
	set file=OBS-git-craftwar-CPU.7z
	rem echo %file%
)

rem echo %favor_arch%
rem echo %file%
rem echo %file_url%
rem pause


set file-new=%file%-new
set _7z=7z.exe
rem set _7z_options=

echo Trying to close OBS (game hooked game should be closed manually)
taskkill /F /FI "WINDOWTITLE eq OBS *" /IM obs64.exe
if exist %file% (
	curl -kLo %file-new% %file_url% -f --retry 5 -z %file%
	if %ERRORLEVEL% EQU 0 (
		if exist %file%-new (
			move /y %file-new% %file%
			%_7z% x %file% -y -o. %_7z_options%
		)
	)
) else (
	curl -kLo %file% %file_url% -f --retry 5 -C -
	if %ERRORLEVEL% EQU 0 (
		%_7z% x %file% -y -o. %_7z_options%
	)
)
if %ERRORLEVEL% NEQ 0 (echo error!)
if "%1" NEQ "noPause" (pause)