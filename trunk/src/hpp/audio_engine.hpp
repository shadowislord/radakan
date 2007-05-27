#ifndef TSL_AUDIO_ENGINE_HPP
#define TSL_AUDIO_ENGINE_HPP

//	#define TSL_FMOD

#ifdef TSL_FMOD
	//	We're using FMOD version 3.
	#include <fmod.h>
#else
	#include <audiere.h>
#endif

#include "set.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{

	///	Sound is the abstract base class for all sounds classes.
	class Sound :
		public Object
	{
		public :
			//	protected constructor(s), see below
			virtual ~Sound ();
			
			//	pure virtual method
			virtual void play () = 0;
			
		protected :
			Sound (string file_name);
	};

	///	for static soundfiles, like .WAV, .MP2, .MP3, .OGG and .RAW
	class Sound_Sample :
		public Sound
	{
		public :
			static Sound_Sample & create (string file_name);

			// These files are closed automatically
			virtual void play ();

		private :
			Sound_Sample (string file_name);
			
			#ifdef TSL_FMOD
				FSOUND_SAMPLE * sample;
			#else
				audiere :: OutputStreamPtr sound;
			#endif
	};

	namespace Engines
	{
		///	Audio_Engine is the music and sound engine.
		class Audio_Engine :
			public Singleton <Audio_Engine>,
			private Set <Sound>
		{
			public :
				Audio_Engine ();
				virtual ~Audio_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				void play ();
				void load (string file_name);

				#ifndef TSL_FMOD
					audiere :: AudioDevicePtr device;
				#endif

			private :
				bool silent;
		};
	}
}

#endif	//	TSL_AUDIO_ENGINE_HPP
