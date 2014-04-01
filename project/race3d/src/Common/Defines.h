#ifndef _DEFINES_H_ 
#define _DEFINES_H_ 

// Safe delete macros
#define SAFE_DELETE( p ) if( p ) { delete p; p = NULL; }

/// Gameversion
#define GAMEVERSION "1.1.0e"
#define GAMEVERSION_NUM 110
#define GAMEVERSION_LETTER e

/// Render Bounding Boxes for objects
//#define ENABLE_RENDER_BBOX
/// Render Bounding Sphere for objects
//#define ENABLE_RENDER_BSPHERE
// Render debug messages from scripts into gameplay log
//#define ENABLE_SCRIPT_MSG_LOG

//#define USE_TRANSITION_EFFECTS

/// Savefile for common parameters
#define CONFIG_COMMON_NAME "savecommon.dat"

/// Paths to resources
#define UI_GROUP_PATH "ui/ui.group"
#define SOUNDS_GROUP_PATH "sounds.group"
#define FONTS_GROUP_PATH "fonts/fonts.group"
#define FONTS_GROUP_PATH_LOW "fonts/fonts_low.group"
#define FONTS_GROUP_PATH_HIGH "fonts/fonts_high.group"

/// Languages
#define LANGPACK_EN_PATH "text/EN.str"
#define LANGPACK_FR_PATH "text/FR.str"
#define LANGPACK_RU_PATH "text/RU.str"
#define LANGPACK_GE_PATH "text/GE.str"
#define LANGPACK_IT_PATH "text/IT.str"
#define LANGPACK_SP_PATH "text/SP.str"

#endif