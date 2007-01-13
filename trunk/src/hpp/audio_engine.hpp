#ifndef AUDIO_ENGINE_HPP
#define AUDIO_ENGINE_HPP

#include "set.hpp"

//	#define TSL_FMOD

#ifdef TSL_FMOD
	#include <fmod.h>
#else
	#include <audiere.h>
#endif

using namespace std;

namespace tsl
{

	/// Sound is the abstract base class for all sounds classes.
	class Sound :
		public Object
	{
		public:
			//	protected constructor(s), see below
			virtual ~Sound ()
			{
			}
			//	pure virtual method
			virtual void play () = 0;
			
		protected:
			Sound (string file_name) :
				Object (file_name)
			{
			}
	};

#ifdef TSL_FMOD
	// .MOD, .S3M, .XM, .IT, .MID, .RMI, .SGT, .FSB
	class Music_Module :
		public Sound
	{
		public:
			Music_Module (string file_name) :
				Sound (file_name)
			{
				module = FMUSIC_LoadSong (file_name . c_str ());
			}
			~Music_Module ()
			{
				FMUSIC_FreeSong (module);
			}
			virtual void play ()
			{
				FMUSIC_PlaySong (module);
			}
			
		private:
			FMUSIC_MODULE * module;
	};
#endif

	// for static soundfiles, like .WAV, .MP2, .MP3, .OGG and .RAW
	class Sound_Sample :
		public Sound
	{
		public:
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
			// These files are closed automatically
			virtual void play ()
			{
				#ifdef TSL_FMOD
					FSOUND_PlaySound (FSOUND_FREE, sample);
				#else
					sound -> play();
				#endif
			}

		private:
			#ifdef TSL_FMOD
				FSOUND_SAMPLE * sample;
			#else
				audiere :: OutputStreamPtr sound;
			#endif
	};

#ifdef TSL_FMOD
	// for streamed audio, like file, url and cd
	class Sound_Stream :
		public Sound
	{
		public:
			Sound_Stream (string file_name) :
				Sound (file_name)
			{
				stream = FSOUND_Stream_Open (file_name . c_str (), 0, 0, 0);
			}
			~Sound_Stream ()
			{
				FSOUND_Stream_Close (stream);
			}
			virtual void play ()
			{
				FSOUND_Stream_Play (FSOUND_FREE, stream);
			}

	private:
		FSOUND_STREAM * stream;
	};
#endif

	template class Set <Sound>;

	class Audio_Engine :
		private Set <Sound>
	{
		public:
			Audio_Engine () :
				Object ("Audio engine"),
				Set <Sound> ("Audio engine")
			{
				#ifdef TSL_FMOD
					FSOUND_Init (44100, 32, 0);
				#else
					device = audiere :: OpenDevice ("");
					if (! device)
					{
						error () << "OpenDevice () failed" << endl;
						abort ();
					}
				#endif
			}
			~Audio_Engine ()
			{
				#ifdef TSL_FMOD
					FSOUND_Close ();
				#endif
			}
			void play ()
			{
				get_child () -> play ();
			};
			void load (string file_name)
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

		#ifndef TSL_FMOD
			private:
				audiere :: AudioDevicePtr device;
		#endif
	};
}

#endif
