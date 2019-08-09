@echo off
cd /d "%~dp0"

set vc_inc_arch=SSE2
call "internal or fewer usages\check_cpu.cmd"

if DEFINED favor_arch (
	set file=OBS-git-craftwar-%favor_arch%-%vc_inc_arch%.7z
	rem use %file% in file_url fails
	rem echo %file%
	set file_url=https://github.com/craftwar/obs-studio/releases/download/git/OBS-git-craftwar-%favor_arch%-%vc_inc_arch%.7z
)

rem echo %favor_arch%
rem echo %vc_inc_arch%
rem echo %file%
rem echo %file_url%
rem pause

echo Checking OBS update
call "update OBS.cmd" noPause
start /D "%cd%\bin\64bit\" obs64.exe
echo Checking updater update
"update updater.cmd"