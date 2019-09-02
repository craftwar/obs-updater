#pragma once

#define STRCMP_CONST(str, const_str) memcmp(str, const_str, sizeof(const_str))
#define ARRAY_LEN(array) (sizeof(array) / sizeof(*array))

#define UPDATER_ROOT_URL \
	"https://github.com/craftwar/obs-updater/releases/download/git/"
static const char *UPDATER_VERSION_URL = UPDATER_ROOT_URL "version.txt";
static const char *UPDATER_URL = UPDATER_ROOT_URL "acraftwar.obs_updater.zip";
#define OBS_ROOT_URL \
	"https://github.com/craftwar/obs-studio/releases/download/git/"

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
	const char *cur_updater = UPDATER_VER;
	char cur_obs[MAX_VERSION_LENGTH];
} ver = {};

void get_cpu_arch();
std::string get_obs_version_url();
const char *get_obs_version_filename();
const char *get_obs_versionFile_path();
std::string get_obs_filename();
std::string get_obs_url();
void get_updater_dir();
//void get_obs_dir();
void get_version(const char *url);
size_t get_version_callback(char *ptr, size_t size, size_t nmemb,
			    void *userdata);
void download_file(const char *url, const char *path);
void extract_file(const char *file, const char *dir);
void update_updater();
