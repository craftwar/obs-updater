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
#include <memory>
#include <direct.h>
#include <curl/curl.h>

//#pragma warning(disable : 4703)
#define STRCMP_CONST(str, const_str) memcmp(str, const_str, sizeof(const_str))
#define STRCMP_CONST_NO_NULL(str, const_str) memcmp(str, const_str, sizeof(const_str) - 1)
#define STRCPY_CONST(dest, src) memcpy(dest, src, sizeof(dest))
#define STRCPY_CONST_NO_NULL(dest, src) memcpy(dest, src, sizeof(dest) - 1)
#define WCSCMP_CONST(str, const_str) wmemcmp(str, const_str, sizeof(const_str) / sizeof(*const_str))
#define WCSCMP_CONST_NO_NULL(str, const_str) \
	wmemcmp(str, const_str, sizeof(const_str) / sizeof(*const_str) - 1)
#define ARRAY_LEN(array) (sizeof(array) / sizeof(*array))
// this applys to L"12345" too
#define TSTRLEN_CONST(str) (sizeof(str) / sizeof(*str) - 1)
#endif //PCH_H
