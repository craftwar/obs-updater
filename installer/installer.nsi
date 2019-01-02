Unicode true
!define APPNAME "craftwar OBS"
Name "${APPNAME}"
RequestExecutionLevel user
OutFile "${APPNAME} installer.exe"

InstallDir "C:\${APPNAME}"

;Page license
;LicenseData LICENSE
Page directory
Page instfiles

SetCompressor /SOLID LZMA
!define VC_redist_dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\${VC_redist_ver}"

Section "OBS Studio"
	setOutPath $INSTDIR
	File /r /x "${APPNAME} installer.exe" /x "*.zip" /x "0_createZip.cmd" craftwar.obs_updater\*
	File "${VC_redist_dir}\vc_redist.x86.exe"
	File "${VC_redist_dir}\vc_redist.x64.exe"
	ExecWait '"$INSTDIR\vc_redist.x86.exe"  /quiet /norestart'
	ExecWait '"$INSTDIR\vc_redist.x64.exe"  /quiet /norestart'
; use cmd.exe /C workaround to pin cmd file
	CreateShortcut "$desktop\craftwar OBS.lnk" '"$SYSDIR\cmd.exe"' '/C "$INSTDIR\update and run OBS-blend.cmd"' "$INSTDIR\obs-studio.ico"
	ExecShell "open" "https://craftwarblog.blogspot.com/2017/09/chatty-obs-by-craftwar.html"
SectionEnd
