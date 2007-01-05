// Starting code to incorporate FMOD into the project by Mike

#include <string>
#include <vector>
#include <fmod.h>

using namespace std;

// .MOD, .S3M, .XM, .IT, .MID, .RMI, .SGT, .FSB
class Music_Module
{

public:

	Music_Module (string file_name) : module (NULL)
	{
		module = FMUSIC_LoadSong (file_name.c_str());
	}

	~Music_Module () { FMUSIC_FreeSong (module); }

	void play () { FMUSIC_PlaySong (module); }

private:

	FMUSIC_MODULE* module;

};

// .WAV, .MP2, .MP3, .OGG or .RAW
// Loads and decodes a static soundfile into memory
class Sound_Sample
{

public:

	Sound_Sample (string file_name) : sample (NULL)
	{
			sample = FSOUND_Sample_Load (FSOUND_FREE, file_name.c_str (), FSOUND_NORMAL,0,0);
	}

	// These files are closed automatically

	void play () { FSOUND_PlaySound (FSOUND_FREE, sample); }

private:

	FSOUND_SAMPLE* sample;

};

// Opens an audio file/url/cd ready for streaming
class Sound_Stream
{

public:

	Sound_Stream () : stream (NULL) { };

	void open (std::string file_name)
	{
		 stream = FSOUND_Stream_Open (file_name.c_str (), 0,0,0);
	}

	void close () {  FSOUND_Stream_Close (stream); }

	void play () { FSOUND_Stream_Play (FSOUND_FREE, stream); }

private:

	FSOUND_STREAM* stream;

};

class Audio_Engine
{

public:

	Audio_Engine () { FSOUND_Init (44100, 32, 0); }
	~Audio_Engine () { FSOUND_Close (); }

	// Unsure of actual Audio Engine implementation
	vector <Music_Module> modules;
	vector <Sound_Sample> samples;
	vector <Sound_Stream> streams;

};
