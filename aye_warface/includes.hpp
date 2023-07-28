#pragma comment(lib, "d3d11.lib")
#define _CRT_SECURE_NO_WARNINGS

//system
#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <immintrin.h>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <map>
#include <d3d11.h>
#include "VMP_SDK/VMProtectSDK.h"

//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

//custom
#include "CustomUtils.hpp"
#include "classes.hpp"
#include "mainrender.hpp"



