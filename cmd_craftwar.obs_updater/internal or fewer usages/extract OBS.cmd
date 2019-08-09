@cd /d "%~dp0"
@set file=OBS-git-craftwar.7z
@set _7z=7z.exe
@%_7z% x %file% -y -o. %_7z_options%
@pause