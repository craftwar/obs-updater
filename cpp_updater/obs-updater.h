#pragma once

//#define test_update

// char:		url, hash in file
// wchar_t:		all else
#ifndef test_update

#define UPDATER_ROOT_URL "https://github.com/craftwar/obs-updater/releases/download/git/"
static const char *UPDATER_VERSION_URL = UPDATER_ROOT_URL "version.txt";
static const char *UPDATER_URL = UPDATER_ROOT_URL "craftwar.obs_updater.zip";

#else

#define UPDATER_ROOT_URL \
	"https://ci.appveyor.com/api/projects/craftwar_appveyor/obs-updater/artifacts/"
static const char *UPDATER_VERSION_URL = UPDATER_ROOT_URL "version.txt"
							  "?branch=dev";
static const char *UPDATER_URL = UPDATER_ROOT_URL "craftwar.obs_updater.zip"
						  "?branch=dev";
#endif

#define OBS_ROOT_URL "https://github.com/craftwar/obs-studio/releases/download/git/"

// UPDATER_VER is set as evironment variable in project preprocessor definition
#ifndef UPDATER_VER
#define UPDATER_VER L"1"
#endif

#define MAX_VERSION_LENGTH 40
#define debug_updater 1

#ifdef NDEBUG
#define OutputDebugStringA(str)
#define OutputDebugStringW(str)
#endif

static struct {
	CURL *curl;
	size_t size;
	wchar_t sha1[MAX_VERSION_LENGTH + 1];
	std::FILE *fp;
	std::wstring branch_name;
	std::wstring vc_inc_arch;
	std::wstring updater_dir;
	std::wstring obs_dir;
	// no git build file, use files signed by Jim to pass anti-cheat
	bool no_wc = false; // don't use git win capture
	bool no_gc = false; // don't use git game capture
	//std::experimental::filesystem::path obs_dir;
} update_info = {};

static struct {
	const wchar_t *const cur_updater = UPDATER_VER;
	wchar_t cur_obs[MAX_VERSION_LENGTH + 1];
} ver = {};

static struct Error {
	Error() : code(0){};
	Error(int code) : code(code){};
	~Error()
	{
		if (code) {
			printf("%s\n", str.c_str());
			system("pause");
		}
	}
	Error(const Error &) = delete;
	Error &operator=(const Error &) = delete;

	int code;
	std::string str;
};
//} error;

void get_cpu_arch();
std::string get_obs_version_url();
const wchar_t *get_obs_version_filename();
const wchar_t *get_obs_versionFile_path();
std::wstring get_obs_filename();
std::wstring get_obs_gc_filename();
std::string get_obs_url();
std::string get_obs_gc_url();
void get_updater_dir();
//void get_obs_dir();
void get_version(const char *url);
size_t get_version_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
void download_file(const char *url, const wchar_t *path);
int extract_file(const wchar_t *file, const wchar_t *dir, const wchar_t *extra_options = nullptr);
void update_updater();
std::unique_ptr<char[]> wstr_to_str(const wchar_t *wstr);
std::unique_ptr<wchar_t[]> str_to_wstr(const char *__restrict str);
