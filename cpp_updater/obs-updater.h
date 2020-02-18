#pragma once

#define STRCMP_CONST(str, const_str) memcmp(str, const_str, sizeof(const_str))
#define ARRAY_LEN(array) (sizeof(array) / sizeof(*array))

//#define test_update

#ifndef test_update

#define UPDATER_ROOT_URL "https://github.com/craftwar/obs-updater/releases/download/git/"
static const char *UPDATER_VERSION_URL = UPDATER_ROOT_URL "version.txt";
static const char *UPDATER_URL = UPDATER_ROOT_URL "craftwar.obs_updater.zip";
#define OBS_ROOT_URL "https://github.com/craftwar/obs-studio/releases/download/git/"

#else

#define UPDATER_ROOT_URL "https://ci.appveyor.com/api/projects/craftwar_appveyor/obs-updater/artifacts/"
static const char* UPDATER_VERSION_URL = UPDATER_ROOT_URL "version.txt" "?branch=dev";
static const char* UPDATER_URL = UPDATER_ROOT_URL "craftwar.obs_updater.zip" "?branch=dev";

#endif



// UPDATER_VER is set as evironment variable in project preprocessor definition
#ifndef UPDATER_VER
#define UPDATER_VER "1"
#endif

#define MAX_VERSION_LENGTH 41
#define debug_updater 1

#ifdef NDEBUG
#define OutputDebugStringA(str)
#endif

static struct {
	CURL *curl;
	size_t size;
	char sha1[MAX_VERSION_LENGTH];
	std::FILE *fp;
	std::string branch_name;
	std::string vc_inc_arch;
	std::string updater_dir;
	std::string obs_dir;
	//std::experimental::filesystem::path obs_dir;
} update_info = {};

static struct {
	const char *const cur_updater = UPDATER_VER;
	char cur_obs[MAX_VERSION_LENGTH];
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
const char *get_obs_version_filename();
const char *get_obs_versionFile_path();
std::string get_obs_filename();
std::string get_obs_url();
void get_updater_dir();
//void get_obs_dir();
void get_version(const char *url);
size_t get_version_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
void download_file(const char *url, const char *path);
void extract_file(const char *file, const char *dir);
void update_updater();
