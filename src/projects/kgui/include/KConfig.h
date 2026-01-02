//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KTypes.h"

//#define KGUY_ENABLE_LOGGER

// Options
#define ID_POOL_START			100
#define KGUI_ID_MDI_FIRSTCHILD	50000

// Default application font settings
#define KGUI_DEF_APP_FONT_NAME	TEXT("Calibri")

// Size in Points
#define KGUI_DEF_APP_FONT_SIZE	9

#ifdef KGUY_ENABLE_LOGGER
	#define MSG_SUCCESS
	#define MSG_WARNING
	#define MSG_ERROR
	#define MSG_LOG
#else
	#define MSG_SUCCESS
	#define MSG_WARNING
	#define MSG_ERROR
	#define MSG_LOG
#endif