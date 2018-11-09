Unicode true
!define APPNAME "craftwar OBS"
Name "${APPNAME}"
RequestExecutionLevel user
OutFile "${APPNAME} installer.exe"

InstallDir "C:\${APPNAME}"

Page directory
Page instfiles

SetCompressor /SOLID LZMA
!define VC_redist_dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\${VC_redist_ver}"

Section "OBS Studio"
	setOutPath $INSTDIR
	File /x "${APPNAME} installer.exe" /x "*.zip" /x "0_createZip.cmd" *.*
	File "${VC_redist_dir}\vc_redist.x86.exe"
	File "${VC_redist_dir}\vc_redist.x64.exe"
	ExecWait '"$INSTDIR\vc_redist.x86.exe"  /quiet /norestart'
	ExecWait '"$INSTDIR\vc_redist.x64.exe"  /quiet /norestart'
	CreateShortcut "$desktop\craftwar OBS.lnk" "$INSTDIR\update and run OBS-blend.cmd" "" "$INSTDIR\obs-studio.ico"
SectionEnd