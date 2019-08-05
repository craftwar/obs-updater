@echo off
cd /d "%~dp0"

set vc_inc_arch=SSE2
call "internal or fewer usages\check_cpu.cmd"
::if DEFINED favor_arch (
::if not "%favor_arch%" == "" (
	set file=OBS-git-craftwar-%favor_arch%-%vc_inc_arch%.7z
	::echo %file%
	::set file=OBS-git-craftwar%favor_arch%.7z
	::echo %file%
	set file_url=https://github.com/craftwar/obs-studio/releases/download/git/%file%
::)
::echo %favor_arch%
::echo %vc_inc_arch%
echo %file_url%
pause
echo Checking OBS update
call "update OBS.cmd" noPause
start /D "%cd%\bin\64bit\" obs64.exe
echo Checking updater update
"update updater.cmd"