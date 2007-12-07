///	All other Radakan headers should (in)directly include this header.

//	Uncomment the next line for release mode.
//	#define NDEBUG

#ifndef NDEBUG
	//	Radakan run with RADAKAN_DEBUG will print debug messages.
	#define RADAKAN_DEBUG
#endif

#include <cassert>
#include <cmath>
#include <string>

#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

#include <OgreVector3.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>

	#if _MSC_VER
		#pragma warning(disable: 4250)
		#pragma warning(disable: 4355)
	#endif

	#define RADAKAN_WINDOWS
#endif

#define RADAKAN_CEGUI_MODE 1
#define RADAKAN_OPENGUI_MODE 2
#define RADAKAN_AUDIERE_MODE 3
#define RADAKAN_FMOD_MODE 4

/*
 * This define sets the audio mode for the whole game.
 *      0       No audio
 *      3       Use Audiere
 *      4       Use FMod
 */
#define RADAKAN_AUDIO_MODE 0

#define RADAKAN_GUI_MODE 1
