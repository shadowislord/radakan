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

#ifdef TSL_FMOD
	///	.MOD, .S3M, .XM, .IT, .MID, .RMI, .SGT, .FSB
	class Music_Module :
		public Sound
	{
		public :
			Music_Module (string file_name);
			~Music_Module ();
			virtual void play ();
			
		private :
			FMUSIC_MODULE * module;
	};
#endif

	///	for static soundfiles, like .WAV, .MP2, .MP3, .OGG and .RAW
	class Sound_Sample :
		public Sound
	{
		public :
			#ifdef TSL_FMOD
				Sound_Sample (string file_name);
			#else
				Sound_Sample (string file_name, audiere :: AudioDevicePtr device);
			#endif

			// These files are closed automatically
			virtual void play ();

		private :
			#ifdef TSL_FMOD
				FSOUND_SAMPLE * sample;
			#else
				audiere :: OutputStreamPtr sound;
			#endif
	};

#ifdef TSL_FMOD
	///	for streamed audio, like file, url and cd
	class Sound_Stream :
		public Sound
	{
		public :
			Sound_Stream (string file_name);
			~Sound_Stream ();
			virtual void play ();

	private :
		FSOUND_STREAM * stream;
	};
#endif

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

		private :
			bool silent;
			#ifndef TSL_FMOD
				audiere :: AudioDevicePtr device;
			#endif
	};
}

#endif	//	TSL_AUDIO_ENGINE_HPP
