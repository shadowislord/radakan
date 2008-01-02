#include "engines/audio_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"

#if RADAKAN_AUDIO_MODE == RADAKAN_FMOD_MODE
	#include <fmod.h>
#endif

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

Sound_Sample ::
	Sound_Sample (string file_name) :
	Object (file_name)
{
	#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
		sound = audiere :: OpenSound (Audio_Engine :: get () -> device . get (), file_name . c_str ());
		if (! sound)
		{
			Engines :: Log :: error (me) << "OpenSound (...) failed" << endl;
			abort ();
		}
	#elif RADAKAN_AUDIO_MODE == RADAKAN_FMOD_MODE
		sample = FSOUND_Sample_Load (FSOUND_FREE, file_name . c_str (), FSOUND_NORMAL, 0, 0);
	#endif
}

void Sound_Sample ::
	play ()
{
	#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
		sound -> play();
	#elif RADAKAN_AUDIO_MODE == RADAKAN_FMOD_MODE
		FSOUND_PlaySound (FSOUND_FREE, sample);
	#endif
}

//	static
string Audio_Engine ::
	get_class_name ()
{
	return "Audio_Engine";
}

Audio_Engine ::
	Audio_Engine () :
	Object ("audio engine")
{
	silent = false;

	#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
		device = audiere :: OpenDevice ("");
		if (! device)
		{
			Engines :: Log :: log (me) << "OpenDevice () failed:" << endl;
			Engines :: Log :: log (me) << "Silence!" << endl;

			silent = true;
		}
	#elif RADAKAN_AUDIO_MODE == RADAKAN_FMOD_MODE
		FSOUND_Init (44100, 32, 0);
	#endif

	load (Settings :: get () -> radakan_path + "/data/sound/prelude_11.ogg");
	play ();

	assert (is_initialized ());
}

Audio_Engine ::
	~Audio_Engine ()
{
	Engines :: Log :: trace (me, Audio_Engine :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();

	#if RADAKAN_AUDIO_MODE == RADAKAN_FMOD_MODE
		if (! silent)
		{
			FSOUND_Close ();
		}
	#endif
}

//	virtual
bool Audio_Engine ::
	is_initialized ()
	const
{
	return Singleton <Audio_Engine> :: is_initialized ();
}

void Audio_Engine ::
	play ()
{
	if (! silent)
	{
		get_child () -> play ();
	}
}

void Audio_Engine ::
	load (string file_name)
{
	if (! silent)
	{
		assert (5 < file_name . size ());
		string extension = file_name . substr (file_name . size () - 3);
		if (extension == "ogg")
		{
			bool check = add (Reference <Sound_Sample> (new Sound_Sample (file_name)));
			assert (check);
		}
		else if (extension == "mp3")
		{
			Engines :: Log :: error (me) << "We do not have the rights to use the .mp3 format. Please use the .ogg format instead." << endl;
			abort ();
		}
		else
		{
			Engines :: Log :: error (me) << "Unknown file format '" << extension << "'" << endl;
			abort ();
		}
	}
}
