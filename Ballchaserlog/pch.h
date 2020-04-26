#pragma once


// Find the required libs here: https://slproweb.com/products/Win32OpenSSL.html
// Win64 OpenSSL v1.1.1g
#pragma comment(lib, "libssl64MT.lib")
#pragma comment(lib, "libcrypto64MT.lib")
#pragma comment(lib, "CRYPT32.lib")
#pragma comment(lib, "BakkesMod.lib")

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "http/httplib.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "imgui/imgui.h"
#include "json/json.hpp"
