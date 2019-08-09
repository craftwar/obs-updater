This is craftwar's customized OBS Studio. (improve performance, fix bug)
Added features: display playing song, VNR translation (read blog for detail)
http://craftwarblog.blogspot.com/2017/09/chatty-obs-by-craftwar.html

I remove some plugins. You need more features, you need install them yourself or use official OBS Studio version instead.
It shares the same settings with official OBS, no extra setup is required.
**Don't install under system folder (ex: C:\Program Files (x86)\), updater can't write. Install in C:\craftwar_OBS or D:\craftwar_OBS ...etc
Don't delete downloaded .7z/zip files, it's used for update check. (delete it only when you encounter download problem)


Microsoft Visual C++ Redistributable for Visual Studio 2019 (x64 and x86) are required.
OBS may work abnormally under older Redistributable. Install 2019 please. (don't uninstall old Redistributable)
https://support.microsoft.com/help/2977003/the-latest-supported-visual-c-downloads

Installer is recommended	https://github.com/craftwar/obs-updater/releases/download/git/craftwar.OBS.installer.exe
It installs C++ Redistributable and create shortcut on desktop.
If you wana pin OBS to taskbar, pin desktop shortcut to taskbar or it won't run updater. (desktop shortcut can be deleted aftern pinned)


"update and run OBS.cmd"	to update and start OBS blend for general users (default in installer, same as "update and run OBS-blend.cmd")
	include most OBS plugins (ex: obs-browser), suitable for 90%+ usage
"update and run OBS-CPU.cmd"	to update and start OBS CPU-specific/debloated version, more optimizations, most funtions is removed.
	It detects INTEL/AMD CPU automatically and downloads CPU-specific version, crash or close on old computers
"get obs-text.cmd"
	download obs-text.dll only (put updater in OBS root directory)

Updater updates craftwar's OBS studio only. (not including plugins you install)
Share updater only, not OBS you get from updating. (CPU specific version is slower in incorrect CPU)


Q:	How do I know I need update plugin?
A:	When OBS version is changed, you may need update it.
	Put newer plugin into OBS to overwrite it or download official OBS and overwite with my updater.

more details (in Traditional Chinese)
http://craftwarblog.blogspot.com/2017/09/chatty-obs-by-craftwar.html


These files should not be used in most cases
"internal or fewer usages" foler: files for updater internal usage, or fewer usage senarios

"update updater.cmd"
	to update updater (including doc, change to auto update updater now)


below docs are not updated

"get obs-text-CPU.cmd"
	more optimized obs-text.dll (may not run on old computers)
"update OBS.cmd"
	to update OBS (auto check AMD/CPU. Pick it manually if it can't detect)
"extract OBS.cmd"
	extract OBS (no download, extract only. use when extract error happens)
"download game capture.cmd"
	download game capture from official OBS
"update and run OBS-AMD.cmd"
	for AMD CPU, higher performance (use only when CPU-specific version can't detect your CPU. lower performance to Intel CPU, use "update and run OBS-INTEL.cmd")
"update OBS -Dev.cmd"
	update OBS Dev (don't use if you are not pro. test only, not always lastest/stable)