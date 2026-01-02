//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <Commctrl.h> // win32 common controls interface
#include <TCHAR.H>
#include <dwmapi.h>
#include <uxtheme.h>

#pragma comment(lib, "Comctl32.lib") // win32 common controls
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "UxTheme")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#include <vector>
#include <memory>
#include <cstdint>

// String type
#include "KString.h"
#include "KColor.h"
#include "KFloat2.h"