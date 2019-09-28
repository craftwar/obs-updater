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
!define VC_redist_dir "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Redist\MSVC\${VC_redist_ver}"

Section "OBS Studio"
	setOutPath $INSTDIR
	File /r /x "${APPNAME} installer.exe" /x "*.zip" /x "0_createZip.cmd" updater\*
	File "${VC_redist_dir}\vc_redist.x86.exe"
	File "${VC_redist_dir}\vc_redist.x64.exe"
	ExecWait '"$INSTDIR\vc_redist.x86.exe"  /quiet /norestart'
	ExecWait '"$INSTDIR\vc_redist.x64.exe"  /quiet /norestart'
; use cmd.exe /C workaround to pin cmd file
	CreateShortcut "$desktop\craftwar OBS.lnk" '$INSTDIR\craftwar-obs-updater.exe'
	ExecShell "open" "https://craftwarblog.blogspot.com/2017/09/obs-studioby-craftwar.html"
SectionEnd
