#include "audio_engine.hpp"

using namespace std;
using namespace tsl;

Sound ::
	Sound (string file_name) :
	Object (file_name)
{
}

Sound ::
	~Sound ()
{
}

#ifdef TSL_FMOD
	Music_Module ::
		Music_Module (string file_name) :
		Sound (file_name)
	{
		module = FMUSIC_LoadSong (file_name . c_str ());
	}
			
	Music_Module ::
		~Music_Module ()
	{
		FMUSIC_FreeSong (module);
	}
			
	void Music_Module ::
		play ()
	{
		FMUSIC_PlaySong (module);
	}
#endif

Sound_Sample ::
	#ifdef TSL_FMOD
		Sound_Sample (string file_name) :
	#else
		Sound_Sample (string file_name, audiere :: AudioDevicePtr device) :
	#endif
	Sound (file_name)
{
	#ifdef TSL_FMOD
		sample = FSOUND_Sample_Load (FSOUND_FREE, file_name . c_str (), FSOUND_NORMAL, 0, 0);
	#else
		sound = audiere :: OpenSound (device, file_name . c_str ());
		if (! sound)
		{
			error () << "OpenSound (...) failed" << endl;
			abort ();
		}
	#endif
}
			
void Sound_Sample ::
	play ()
{
	#ifdef TSL_FMOD
		FSOUND_PlaySound (FSOUND_FREE, sample);
	#else
		sound -> play();
	#endif
}

#ifdef TSL_FMOD
	Sound_Stream ::
		Sound_Stream (string file_name) :
		Sound (file_name)
	{
		stream = FSOUND_Stream_Open (file_name . c_str (), 0, 0, 0);
	}
	
	Sound_Stream ::
		~Sound_Stream ()
	{
		FSOUND_Stream_Close (stream);
	}
	
	void Sound_Stream ::
		play ()
	{
		FSOUND_Stream_Play (FSOUND_FREE, stream);
	}
#endif

Audio_Engine ::
	Audio_Engine () :
	Object ("Audio engine")
{
	silent = false;

	#ifdef TSL_FMOD
		FSOUND_Init (44100, 32, 0);
	#else
		device = audiere :: OpenDevice ("");
		if (! device)
		{
			debug () << "OpenDevice () failed:" << endl;
			debug () << "Silence!" << endl;

			silent = true;
		}
	#endif
}

Audio_Engine ::
	~Audio_Engine ()
{
	assert (is_initialized ());
	trace () << "~" << get_class_name () << " ()" << endl;
	if (! silent)
	{
		#ifdef TSL_FMOD
			FSOUND_Close ();
		#endif
	}
}

//	virtual
bool Audio_Engine ::
	is_initialized ()
	const
{
	return warn <Audio_Engine> (Singleton <Audio_Engine> :: is_initialized ());
}

//	static
string Audio_Engine ::
	get_class_name ()
{
	return "Audio_Engine";
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
			add ((new Sound_Sample (file_name
			#ifndef TSL_FMOD
				, device
			#endif
			)) -> to_type <Sound> ());
		}
		else if (extension == "mp3")
		{
			error () << "- We do not have the rights to use the .mp3 format. Please use the .ogg format instead." << endl;
			abort ();
		}
		else
		{
			error () << "doesn't know the file format '" << extension << "'" << endl;
			abort ();
		}
	}
}
