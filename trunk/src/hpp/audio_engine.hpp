// Starting code to incorporate FMOD into the project by Mike

#include <string>
#include <vector>
#include <fmod.h>

// .MOD, .S3M, .XM, .IT, .MID, .RMI, .SGT, .FSB
class Music_Module
{

public:

	Music_Module () : module (NULL) { }

	void load (std::string file_name)
	{
		module = FMUSIC_LoadSong (file_name.c_str(););
	}

	void free () { FMUSIC_FreeSong (module); }

	void play () { FMUSIC_PlaySong (module); }

protected:

	FMUSIC_MODULE* module;

};

// .WAV, .MP2, .MP3, .OGG or .RAW
// Loads and decodes a static soundfile into memory
class Sound_Sample
{

public:

	Sound_Sample () : sample (NULL) { };

	void load (std::string file_name)
	{
		sample = FSOUND_Sample_Load (FSOUND_FREE, file_name.c_str (), FSOUND_NORMAL);
	}

	// These files are closed automatically

	void play () { FSound_PlaySound (FSOUND_FREE, sample); }

protected:

	FSOUND_SAMPLE* sample;

};

// Opens an audio file/url/cd ready for streaming
class Sound_Stream
{

public:

	Sound_Stream () : stream (NULL) { };

	void open (std::string file_name)
	{
		 stream = FSOUND_Stream_Open (file_name.c_str (), 0);
	}

	void close () {  FSOUND_Stream_Close (stream); }

	void play () { FSound_Stream_Play (FSOUND_FREE, stream); }

protected:

	FSOUND_STREAM* stream;

};

class Audio_Engine
{

public:

	void init_fmod () { FSOUND_Init (44100, 32, 0); }
	void shutdown_fmod () { FSOUND_Close (); }

protected:

	// Unsure of actual Audio Engine implementation
	std::vector<Music_Module> modules;
	std::vector<Sound_Sample> samples;
	std::vector<Sound_Stream> streams;

};