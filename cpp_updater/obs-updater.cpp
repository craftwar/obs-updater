#include "pch.h"
#include "obs-updater.h"

// https://github.com/obsproject/obs-studio/blob/dd3ed096f891742e0c35f8237e198edab7377b45/deps/file-updater/file-updater/file-updater.c

#define STRCPY_CONST(dest, src) memcpy(dest, src, sizeof(dest))
#define STRCPY_CONST_NO_NULL(dest, src) memcpy(dest, src, sizeof(dest)-1)

// use __cpuid? better information
// https://docs.microsoft.com/cpp/intrinsics/cpuid-cpuidex
void get_cpu_arch()
{
	const char *brand = std::getenv("PROCESSOR_IDENTIFIER");
	if (strstr(brand, "AMD"))
		update_info.branch_name = "AMD64-";
	else
		update_info.branch_name = "INTEL64-";
	if (!update_info.vc_inc_arch.length())
		update_info.vc_inc_arch = "SSE2";
}

// https://github.com/craftwar/obs-studio/releases/download/git/AMD64-AVX2.txt
std::string get_obs_version_url()
{
	static std::string obs_version_url(OBS_ROOT_URL);
	obs_version_url += get_obs_version_filename();
	return obs_version_url;
}

std::string get_obs_filename()
{
	static std::string filename;
	if (!filename.length()) {
		filename = "OBS-git-craftwar-";
		filename += update_info.branch_name;
		filename += update_info.vc_inc_arch;
		filename += ".7z";
	}
	return filename;
}

//https://github.com/craftwar/obs-studio/releases/download/git/OBS-git-craftwar-AMD64-AVX2.7z
std::string get_obs_url()
{
	return std::string(OBS_ROOT_URL + get_obs_filename());
}

// include '\'
void get_updater_dir()
{
	char *last = strrchr(_pgmptr, '\\');
	//OutputDebugStringA(_pgmptr);
	//OutputDebugStringA("\n");
	update_info.updater_dir =
		std::move(std::string(_pgmptr, last - _pgmptr + 1));
	//OutputDebugStringA(update_info.updater_dir.c_str());
	//OutputDebugStringA("\n");
	//if (update_info.updater_dir.find("C:\Program Files")!=  std::string::npos)
	//	;
}

//void get_obs_dir()
//{
//	update_info.obs_dir = "";
//}

void get_version(const char *url)
{
	curl_easy_setopt(update_info.curl, CURLOPT_WRITEFUNCTION,
			 get_version_callback);
	//curl_easy_setopt(update_info.curl, CURLOPT_WRITEDATA, NULL);
	update_info.size = 0;
	curl_easy_setopt(update_info.curl, CURLOPT_URL, url);
	curl_easy_perform(update_info.curl);
	update_info.sha1[update_info.size] = 0;
}

size_t get_version_callback(char *ptr, size_t size, size_t nmemb,
			    void *userdata)
{
	if (update_info.size + nmemb <= sizeof(update_info.sha1)) {
		memcpy(update_info.sha1 + update_info.size, ptr, nmemb);
		update_info.size += nmemb;

		return nmemb;
	} else
		return 0;
}

const char *get_obs_version_filename()
{
	static std::string fileName;
	if (!fileName.length())
		fileName = std::move(update_info.branch_name +
				     update_info.vc_inc_arch + ".txt");

	return fileName.c_str();
}

const char *get_obs_versionFile_path()
{
	static std::string path;
	OutputDebugStringA(path.c_str());
	OutputDebugStringA("\n");
	if (!path.length())
		path = std::move(update_info.obs_dir +
				 get_obs_version_filename());
	OutputDebugStringA(path.c_str());
	OutputDebugStringA("\n");
	return path.c_str();
}

void read_obs_version()
{
	std::unique_ptr<std::FILE, decltype(std::fclose) *> file{
		std::fopen(get_obs_versionFile_path(), "r"), std::fclose};
	if (file.get())
		fgets(ver.cur_obs, ARRAY_LEN(ver.cur_obs), file.get());
}

void write_obs_version()
{
	std::unique_ptr<std::FILE, decltype(std::fclose) *> file{
		std::fopen(get_obs_versionFile_path(), "w"), std::fclose};
	if (file.get())
		fputs(update_info.sha1, file.get());
	else
		printf("can't write obs_versionFile\n");
}

void download_file(const char *url, const char *path)
{
	curl_easy_setopt(update_info.curl, CURLOPT_URL, url);
	std::unique_ptr<std::FILE, decltype(std::fclose) *> file{
		std::fopen(path, "wb"), std::fclose};
	curl_easy_setopt(update_info.curl, CURLOPT_WRITEDATA, file.get());
	curl_easy_setopt(update_info.curl, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_perform(update_info.curl);
}

// %_7z% x %file% -y -o. %_7z_options%
void extract_file(const char *file, const char *outputDir)
{
	std::string cmd("7z.exe x ");
	cmd += file;
	cmd += " -y -o";
	cmd += outputDir;
	//std::system("dir");
	std::system(cmd.c_str());
}

void exec_program(LPSTR lpCommandLine)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessA(NULL,          // No module name (use command line)
			    lpCommandLine, // Command line
			    NULL,          // Process handle not inheritable
			    NULL,          // Thread handle not inheritable
			    FALSE,         // Set handle inheritance to FALSE
			    0,             // No creation flags
			    NULL,          // Use parent's environment block
			    NULL,          // Use parent's starting directory
			    &si,           // Pointer to STARTUPINFO structure
			    &pi) // Pointer to PROCESS_INFORMATION structure
	) {
		printf("CreateProcess failed (%lu).\n", GetLastError());
		printf("%s", lpCommandLine);
		system("pause");
		exit(1);
	}
	// auto-closed when program end
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}

void update_updater()
{
	auto const end = std::filesystem::directory_iterator();
	for (std::filesystem::directory_iterator iter(
		     update_info.updater_dir);
	     iter != end; ++iter) {
		try {
			std::filesystem::copy(
				*iter, update_info.obs_dir,
				std::filesystem::copy_options::
					create_hard_links);
		} catch (const std::filesystem::filesystem_error
				 &error) {
			printf("[Error] copy from %s to %s: %s\n",
			       error.path1().c_str(), error.path2().c_str(),
			       error.what());
			system("pause");
			exit(1);
		}
	}

	for (std::filesystem::directory_iterator iter(
		     update_info.updater_dir);
	     iter != end; ++iter) {
		try {
			std::filesystem::remove(*iter);
		} catch (const std::filesystem::filesystem_error
				 &error) {
			printf("[Error] delete %s: %s\n", error.path1().c_str(),
			       error.what());
			system("pause");
			exit(1);
		}
	}
}

int main(int argc, char *argv[])
{
	const char mutexName[] = "Local\\craftwar's OBS updater " UPDATER_VER;
	HANDLE mutex = CreateMutexA(NULL, TRUE, mutexName);
#ifdef _DEBUG
	DWORD error = GetLastError();
#endif
	if (mutex == NULL)
		exit(1);
	//enum class Mode : unsigned char { check, update } mode;
	static bool bUpdateUpdater = false;

	// parse updater args
	{
		char **const end = argv + argc;
		for (char **arg = argv + 1; arg < end; ++arg) {
			OutputDebugStringA("arg\n");
			OutputDebugStringA(*arg);
			OutputDebugStringA("\n");
			if (STRCMP_CONST_NO_NULL(*arg, "-updater_ver") == 0) {
				if (strcmp(*(++arg), ver.cur_updater) != 0) {
					bUpdateUpdater = true;
					OutputDebugStringA(
						"Running newer updater...\n");
				}
			} else if (STRCMP_CONST_NO_NULL(*arg, "-vc_inc_arch") ==
				   0)
				update_info.vc_inc_arch = *(++arg);
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
	_chdir(update_info.updater_dir.c_str());

	if (!bUpdateUpdater) {
		printf("%s Checking updater version...\n", ver.cur_updater);
		get_version(UPDATER_VERSION_URL);
		if (memcmp(ver.cur_updater, update_info.sha1,
			   update_info.size + 1) != 0) {
			printf("%s -> %s Getting newer updater...\n",
			       ver.cur_updater, update_info.sha1);
			static char path[] = "craftwar.obs_updater.zip";
			download_file(UPDATER_URL, path);
			extract_file(path, "update");
			std::string cmd =
				"update\\craftwar-obs-updater.exe -updater_ver ";
			cmd += ver.cur_updater;

			exec_program((LPSTR)cmd.c_str());

			return 0;
		} else {
			update_info.obs_dir = update_info.updater_dir;
			OutputDebugStringA(update_info.obs_dir.c_str());
			OutputDebugStringA(
				"update_info.obs_dir = update_info.updater_dir;\n");
		}
	} else { // bUpdateUpdater == true
		const size_t length = update_info.updater_dir.length();
		OutputDebugStringA(update_info.updater_dir.c_str());
		OutputDebugStringA("update_info.updater_dir()\n");
		const size_t index =
			update_info.updater_dir.find_last_of('\\', length - 2);
		update_info.obs_dir =
			update_info.updater_dir.substr(0, index + 1);
		OutputDebugStringA(update_info.obs_dir.c_str());
		OutputDebugStringA("update_info.obs_dir.c_str()\n");
	}

	printf("Checking OBS version...\n");
	get_version(get_obs_version_url().c_str());
	read_obs_version();
	if (memcmp(ver.cur_obs, update_info.sha1, update_info.size + 1) != 0) {
		printf("%s -> %s Getting newer OBS...\n", ver.cur_obs,
		       update_info.sha1);
		std::string path = update_info.obs_dir + get_obs_filename();
		download_file(get_obs_url().c_str(), path.c_str());
		extract_file(path.c_str(), update_info.obs_dir.c_str());
		write_obs_version();
	}
	curl_easy_cleanup(update_info.curl);
	curl_global_cleanup();

	std::string obs_bin_dir = update_info.obs_dir + "bin\\64bit";
	_chdir(obs_bin_dir.c_str());
	printf("Starting OBS...\n");
	//system("obs64.exe");
	exec_program((LPSTR) "obs64.exe");

	if (bUpdateUpdater) {
		printf("Updating newer updater...\n");
		update_updater();
	}

	return 0;
}
