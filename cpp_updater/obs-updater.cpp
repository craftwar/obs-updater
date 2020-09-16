#include "pch.h"
#include "obs-updater.h"
#include <locale>

#define IGNORE_WC L" -x!obs-plugins\\64bit\\win-capture.dll"
#define IGNORE_GC L" \"-x!data\\obs-plugins\\win-capture\\*\""

// https://github.com/obsproject/obs-studio/blob/dd3ed096f891742e0c35f8237e198edab7377b45/deps/file-updater/file-updater/file-updater.c

// use __cpuid? better information
// https://docs.microsoft.com/cpp/intrinsics/cpuid-cpuidex
void get_cpu_arch()
{
	const wchar_t *brand = _wgetenv(L"PROCESSOR_IDENTIFIER");
	if (wcsstr(brand, L"AMD"))
		update_info.branch_name = L"AMD64-";
	else
		update_info.branch_name = L"INTEL64-";
	if (!update_info.vc_inc_arch.length())
		update_info.vc_inc_arch = L"SSE2";
}

// https://github.com/craftwar/obs-studio/releases/download/git/AMD64-AVX2.txt
std::string get_obs_version_url()
{
	static std::string obs_version_url(OBS_ROOT_URL);
	std::unique_ptr<char[]> str = wstr_to_str(get_obs_version_filename());
	obs_version_url += str.get();
	return obs_version_url;
}

std::wstring get_obs_filename()
{
	static std::wstring filename;
	if (!filename.length()) {
		filename = L"OBS-git-craftwar-";
		filename += update_info.branch_name;
		filename += update_info.vc_inc_arch;
		filename += L".7z";
	}
	return filename;
}

std::wstring get_obs_gc_filename()
{
	return L"gc.7z";
}

//https://github.com/craftwar/obs-studio/releases/download/git/OBS-git-craftwar-AMD64-AVX2.7z
std::string get_obs_url()
{
	std::string str(OBS_ROOT_URL);
	std::unique_ptr<char[]> filename = wstr_to_str(get_obs_filename().c_str());
	str += filename.get();
	return str;
}

//https://github.com/craftwar/obs-studio/releases/download/git/gc-AMD64.7z
std::string get_obs_gc_url()
{
	std::string str(OBS_ROOT_URL "gc.7z");
	return str;
}

// include '\'
void get_updater_dir()
{
	wchar_t *last = wcsrchr(_wpgmptr, L'\\');
	//OutputDebugStringW(_pgmptr);
	//OutputDebugStringW("\n");
	update_info.updater_dir = std::move(std::wstring(_wpgmptr, last - _wpgmptr + 1));
	//OutputDebugStringW(update_info.updater_dir.c_str());
	//OutputDebugStringW("\n");
	//if (update_info.updater_dir.find("C:\Program Files")!=  std::string::npos)
	//	;
}

//void get_obs_dir()
//{
//	update_info.obs_dir = "";
//}

void get_version(const char *url)
{
	printf("Getting version file from %s\n", url);
	curl_easy_setopt(update_info.curl, CURLOPT_WRITEFUNCTION, get_version_callback);
	//curl_easy_setopt(update_info.curl, CURLOPT_WRITEDATA, NULL);
	update_info.size = 0;
	curl_easy_setopt(update_info.curl, CURLOPT_URL, url);
	curl_easy_setopt(update_info.curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_perform(update_info.curl);

	if (verify_download())
		update_info.sha1[update_info.size] = 0;
	else
		update_info.sha1[0] = 0;
}

size_t get_version_callback(char *__restrict ptr, size_t size, size_t nmemb, void *userdata)
{
	if (update_info.size + nmemb < ARRAY_LEN(update_info.sha1)) {
		wchar_t *__restrict wptr = update_info.sha1 + update_info.size;
		wchar_t *end = wptr + nmemb;
		while (wptr < end)
			*wptr++ = *ptr++;
		update_info.size += nmemb;

		return nmemb;
	} else
		return 0;
}

const wchar_t *get_obs_version_filename()
{
	static std::wstring fileName;
	if (!fileName.length())
		fileName = std::move(update_info.branch_name + update_info.vc_inc_arch + L".txt");

	return fileName.c_str();
}

const wchar_t *get_obs_versionFile_path()
{
	static std::wstring path;
	OutputDebugStringW(path.c_str());
	OutputDebugStringW(L"\n");
	if (!path.length())
		path = std::move(update_info.obs_dir + get_obs_version_filename());
	OutputDebugStringW(path.c_str());
	OutputDebugStringW(L"\n");
	return path.c_str();
}

void read_obs_version()
{
	std::unique_ptr<std::FILE, decltype(std::fclose) *> file{
		_wfopen(get_obs_versionFile_path(), L"r"), std::fclose};
	if (file.get()) {
		int c;
		wchar_t *__restrict ptr = ver.cur_obs;
		while ((c = std::fgetc(file.get())) != EOF)
			*ptr++ = c;
	}
}

void write_obs_version()
{
	std::unique_ptr<std::FILE, decltype(std::fclose) *> file{
		_wfopen(get_obs_versionFile_path(), L"w"), std::fclose};
	if (file.get()) {
		wchar_t *__restrict ptr = update_info.sha1;
		while (*ptr)
			std::fputc(*ptr++, file.get());
	} else
		printf("can't write obs_versionFile\n");
}

void download_file(const char *url, const wchar_t *path)
{
	printf("Downloading file from %s\n", url);
	curl_easy_setopt(update_info.curl, CURLOPT_URL, url);
	std::unique_ptr<std::FILE, decltype(std::fclose) *> file{_wfopen(path, L"wb"), std::fclose};
	curl_easy_setopt(update_info.curl, CURLOPT_WRITEDATA, file.get());
	curl_easy_setopt(update_info.curl, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(update_info.curl, CURLOPT_NOPROGRESS, 0);
	curl_easy_perform(update_info.curl);
}

// check if download is successful
bool verify_download()
{
	curl_off_t cl;
	curl_easy_getinfo(update_info.curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &cl);
	curl_off_t dl;
	curl_easy_getinfo(update_info.curl, CURLINFO_SIZE_DOWNLOAD_T, &dl);

	return dl != -1 && cl == dl;
}

// %_7z% x %file% -y -o. %_7z_options%
int extract_file(const wchar_t *file, const wchar_t *outputDir, const wchar_t *extra_options)
{
	std::wstring cmd(L"7z.exe x \"");
	cmd += file;
	cmd += L"\" -y -o\"";
	cmd += outputDir;
	cmd += L'\"';
	if (extra_options)
		cmd += extra_options;
	//std::system("dir");
	int error = _wsystem(cmd.c_str());
	if (error) {
		printf("[Error=%d] when extracting \"%ls\" to \"%ls\"\n", error, file, outputDir);
		std::system("pause");
		//exit(error);
	}
	return error;
}

void exec_program(LPWSTR lpCommandLine)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessW(NULL,          // No module name (use command line)
			    lpCommandLine, // Command line
			    NULL,          // Process handle not inheritable
			    NULL,          // Thread handle not inheritable
			    FALSE,         // Set handle inheritance to FALSE
			    0,             // No creation flags
			    NULL,          // Use parent's environment block
			    NULL,          // Use parent's starting directory
			    &si,           // Pointer to STARTUPINFO structure
			    &pi)           // Pointer to PROCESS_INFORMATION structure
	) {
		printf("CreateProcess failed (%lu).\n", GetLastError());
		printf("%ls", lpCommandLine);
		std::system("pause");
		exit(1);
	}
	// auto-closed when program end
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}

void update_updater()
{
	const std::filesystem::path updater_path(update_info.updater_dir);
	std::string::size_type pos = update_info.updater_dir.find_last_of('\\');
	//if (pos != std::string::npos)
	pos = update_info.updater_dir.find_last_of('\\', pos - 1);
	const std::filesystem::path target_path(update_info.updater_dir.substr(0, pos));
	auto const end = std::filesystem::directory_iterator();
	for (std::filesystem::directory_iterator iter(updater_path); iter != end; ++iter) {
		try {
			// create_hard_links fails when file exits (even overwrite is on)
			std::filesystem::copy(iter->path(), target_path,
					      std::filesystem::copy_options::overwrite_existing
					      //|std::filesystem::copy_options::create_hard_links
			);
		} catch (const std::filesystem::filesystem_error &error) {
			std::printf("[Error] copy from %ls to %ls: %s\n",
				    error.path1().wstring().c_str(),
				    error.path2().wstring().c_str(), error.what());
			std::system("pause");
			exit(1);
		}
	}

	// %Ts %hs?? way to specify string type? wide/byte? (M$-specific)
	// %hs M$-specific means char*
	for (std::filesystem::directory_iterator iter(updater_path); iter != end; ++iter) {
		try {
			std::filesystem::remove(*iter);
		} catch (const std::filesystem::filesystem_error &error) {
			std::printf("[Error] delete %ls: %s\n", error.path1().wstring().c_str(),
				    error.what());
			std::system("pause");
			exit(1);
		}
	}
}

std::unique_ptr<char[]> wstr_to_str(const wchar_t *__restrict wstr)
{
	std::unique_ptr<char[]> str;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, NULL, NULL);
	if (len) {
		str.reset(new char[++len]);
		if (str && WideCharToMultiByte(CP_ACP, 0, wstr, -1, str.get(), len, NULL, NULL))
			return str;
	}
	return nullptr;
}

std::unique_ptr<wchar_t[]> str_to_wstr(const char *__restrict str)
{
	std::unique_ptr<wchar_t[]> wstr;
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
	if (len) {
		wstr.reset(new wchar_t[++len]);
		if (wstr && MultiByteToWideChar(CP_ACP, 0, str, -1, wstr.get(), len))
			return wstr;
	}
	return nullptr;
}

int wmain(int argc, wchar_t *__restrict argv[])
{
	//std::setlocale(LC_ALL, "");
	// replace the C++ global locale as well as the C locale with the user-preferred locale
	std::locale::global(std::locale(""));
	//wchar_t *const *const end = argv + argc;
	//printf("argv = ");
	//for (wchar_t ** __restrict it = argv; it < end; ++it)
	//	printf("%ls", *it);
	std::system("cd"); // for debug
	// limit single instance to specific updater version
	const wchar_t mutexName[] = L"Local\\craftwar's OBS updater " UPDATER_VER;
	HANDLE mutex = CreateMutexW(NULL, TRUE, mutexName);
#ifdef _DEBUG
	DWORD error = GetLastError();
#endif
	if (mutex == NULL)
		exit(1);
	//enum class Mode : unsigned char { check, update } mode;
	static bool bUpdatedUpdater = false;

	// parse updater args
	std::wstring extra_parameters;
	{
		wchar_t **const end = argv + argc;
		for (wchar_t **arg = argv + 1; arg < end; ++arg) {
			OutputDebugStringW(L"arg\n");
			OutputDebugStringW(*arg);
			OutputDebugStringW(L"\n");
			if (WCSCMP_CONST_NO_NULL(*arg, L"-updater_ver") == 0) {
				if (wcscmp(*(++arg), ver.cur_updater) != 0) {
					bUpdatedUpdater = true;
					OutputDebugStringW(L"Running newer updater...\n");
				}
			} else if (WCSCMP_CONST_NO_NULL(*arg, L"-vc_inc_arch") == 0) {
				update_info.vc_inc_arch = *(++arg);
			} else if (WCSCMP_CONST_NO_NULL(*arg, L"-wc") == 0) {
				if (!WCSCMP_CONST_NO_NULL(*arg, L"no"))
					update_info.wc = file_source::no;
				else if (!WCSCMP_CONST_NO_NULL(*arg, L"jim"))
					update_info.wc = file_source::jim;
			} else if (WCSCMP_CONST_NO_NULL(*arg, L"-gc") == 0) {
				if (!WCSCMP_CONST_NO_NULL(*arg, L"no"))
					update_info.gc = file_source::no;
				else if (!WCSCMP_CONST_NO_NULL(*arg, L"jim"))
					update_info.gc = file_source::jim;
			} else {
				extra_parameters += L' ';
				extra_parameters += *arg;
			}
			//else if (STRCMP_CONST_NO_NULL(arg, "-obs_ver") == 0)
			//	ver.cur_obs = *(++arg);
		}
	}
	//printf("Trying to close OBS (game hooked game should be closed manually)\n");
	//system("taskkill /F /FI \"WINDOWTITLE eq OBS* \" /IM obs64.exe");
	// taskkill /F /IM "craftwar-obs-updater.exe"

	curl_global_init(CURL_GLOBAL_ALL);
	update_info.curl = curl_easy_init();
	//curl_easy_setopt(update_info.curl, CURLOPT_SSL_ENABLE_ALPN, 0);
	if (!update_info.curl)
		return 1;

	curl_easy_setopt(update_info.curl, CURLOPT_ACCEPT_ENCODING, "");
	curl_easy_setopt(update_info.curl, CURLOPT_FOLLOWLOCATION, 1);

	get_cpu_arch();
	get_updater_dir();
	_wchdir(update_info.updater_dir.c_str());

	if (!bUpdatedUpdater) {
		printf("%ls Checking updater version...\n", ver.cur_updater);
		get_version(UPDATER_VERSION_URL);
		if (update_info.size &&
		    wmemcmp(ver.cur_updater, update_info.sha1, update_info.size + 1) != 0) {
			printf("%ls -> %ls Getting newer updater...\n", ver.cur_updater,
			       update_info.sha1);
			static wchar_t path[] = L"craftwar.obs_updater.zip";
			download_file(UPDATER_URL, path);
			if (verify_download()) {
				extract_file(path, L"update");
				std::wstring cmd =
					L"update\\craftwar-obs-updater.exe -updater_ver ";
				cmd += ver.cur_updater;
				cmd += L" -vc_inc_arch ";
				cmd += update_info.vc_inc_arch;

				exec_program(cmd.data());

				return 0;
			} else {
				printf("newer updater download is failed, use current updater! This may cause problem.\n");
			}
		}
		update_info.obs_dir = update_info.updater_dir;
		OutputDebugStringW(update_info.obs_dir.c_str());
		OutputDebugStringW(L"update_info.obs_dir = update_info.updater_dir;\n");

	} else { // bUpdatedUpdater == true
		const size_t length = update_info.updater_dir.length();
		OutputDebugStringW(update_info.updater_dir.c_str());
		OutputDebugStringW(L"update_info.updater_dir()\n");
		const size_t index = update_info.updater_dir.find_last_of('\\', length - 2);
		update_info.obs_dir = update_info.updater_dir.substr(0, index + 1);
		OutputDebugStringW(update_info.obs_dir.c_str());
		OutputDebugStringW(L"update_info.obs_dir.c_str()\n");
	}

	printf("Checking OBS version...\n");
	get_version(get_obs_version_url().c_str());
	read_obs_version();
	if (update_info.size && wmemcmp(ver.cur_obs, update_info.sha1, update_info.size + 1) != 0) {
		printf("%ls -> %ls Getting newer OBS...\n", ver.cur_obs, update_info.sha1);
		std::wstring path = update_info.obs_dir + get_obs_filename();
		download_file(get_obs_url().c_str(), path.c_str());
		std::wstring ignore_options;
		if (update_info.wc != file_source::std)
			ignore_options += IGNORE_WC;
		if (update_info.gc != file_source::std)
			ignore_options += IGNORE_GC;
		if (!extract_file(path.c_str(), update_info.obs_dir.c_str(),
				  ignore_options.c_str())) {
			write_obs_version();
			if (update_info.wc == file_source::jim ||
			    update_info.gc == file_source::jim) {
				path = update_info.obs_dir + get_obs_gc_filename();
				//xxx handle broken download, download only if file is updated
				download_file(get_obs_gc_url().c_str(), path.c_str());
				std::wstring ignore_options;
				if (update_info.wc != file_source::jim)
					ignore_options += IGNORE_WC;
				if (update_info.gc != file_source::jim)
					ignore_options += IGNORE_GC;
				extract_file(path.c_str(), update_info.obs_dir.c_str(),
					     ignore_options.c_str());
			}
		}
	}
	curl_easy_cleanup(update_info.curl);
	curl_global_cleanup();

	std::wstring obs_bin_dir = update_info.obs_dir + L"bin\\64bit";
	_wchdir(obs_bin_dir.c_str());
	printf("Starting OBS...\n");
	//system("obs64.exe");
	std::wstring obs_exe(L"obs64.exe"); // CreateProcessW can modify the contents of this string
	obs_exe += extra_parameters;
	exec_program(obs_exe.data());

	if (bUpdatedUpdater) {
		printf("Updating newer updater...\n");
		_wchdir(update_info.updater_dir.c_str());
		std::system("move /y *.* ..");
		//update_updater();
	}

	return 0;
}
