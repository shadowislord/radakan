#ifndef RADAKAN_AUDIO_ENGINE_HPP
#define RADAKAN_AUDIO_ENGINE_HPP

#ifdef RADAKAN_FMOD
	class FSOUND_SAMPLE;
#else
	#include <audiere.h>
#endif

#include "set.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{

	///	for static soundfiles, like .WAV, .MP2, .MP3, .OGG and .RAW
	class Sound_Sample :
		public virtual Object
	{
		public :
			Sound_Sample (string file_name);
			
			// These files are closed automatically
			virtual void play ();

		private :
			#ifdef RADAKAN_FMOD
				boost :: shared_ptr <FSOUND_SAMPLE> sample;
			#else
				audiere :: OutputStreamPtr sound;
			#endif
	};

	namespace Engines
	{
		///	Audio_Engine is the music and sound engine.
		class Audio_Engine :
			public Singleton <Audio_Engine>,
			private Set <Sound_Sample>
		{
			public :
				Audio_Engine ();
				virtual ~Audio_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				void play ();
				void load (string file_name);

				#ifndef RADAKAN_FMOD
					audiere :: AudioDevicePtr device;
				#endif

			private :
				bool silent;
		};
	}
}

#endif	//	RADAKAN_AUDIO_ENGINE_HPP
