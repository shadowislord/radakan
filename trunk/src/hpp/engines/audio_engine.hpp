#ifndef RADAKAN_AUDIO_ENGINE_HPP
#define RADAKAN_AUDIO_ENGINE_HPP

#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	#include "singleton.hpp"

	#include <audiere.h>

	using namespace std;

	namespace Radakan
	{
		template <typename T, class U> class Map;

		///	for static soundfiles, like .WAV, .MP2, .MP3, .OGG and .RAW
		class Sound_Sample :
			public virtual Object
		{
			public :
				Sound_Sample (string file_name);
				
				// These files are closed automatically
				virtual void play ();

			private :
				audiere :: OutputStreamPtr sound;
		};

		namespace Engines
		{
			///	Audio_Engine is the music and sound engine.
			class Audio_Engine :
				public Singleton <Audio_Engine>
			{
				public :
					static string get_class_name ();
					
					Audio_Engine ();
					virtual ~Audio_Engine ();
					virtual bool is_initialized () const;
					
					void play ();
					void load (string file_name);

					audiere :: AudioDevicePtr device;

				private :
					bool silent;
					Reference <Map <string, Sound_Sample> > sounds;
			};
		}
	}
	#endif

#endif	//	RADAKAN_AUDIO_ENGINE_HPP
