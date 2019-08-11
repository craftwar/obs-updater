// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#define _CRT_SECURE_NO_WARNINGS
// add headers that you want to pre-compile here

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <direct.h>
#include <curl/curl.h>

//#pragma warning(disable : 4703)
#define STRCMP_CONST(str, const_str) memcmp(str, const_str, sizeof(const_str))
#define STRCMP_CONST_NO_NULL(str, const_str) \
	memcmp(str, const_str, sizeof(const_str) - 1)
// this applys to L"12345" too
#define TSTRLEN_CONST(str) (sizeof(str) / sizeof(*str) - 1)
#endif //PCH_H
