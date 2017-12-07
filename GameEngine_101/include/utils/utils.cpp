/*
==================================================================================================================================================================
|Copyright © 2017 Oscar Lara	- scarlara@hotmail.com																											   |
|																																								   |
|See individual libraries separate legal notices																												   |
|																																								   |
|Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),				   |
|to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,			       |
|and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :				       |
|																																								   |
|The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.								       |
|																																								   |
|THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			   |
|FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,	   |
|WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.	       |
==================================================================================================================================================================
*/
#include "utils.h"

HANDLE hStdout;
CONSOLE_CURSOR_INFO lpCursor;
COORD coord = { 0,0 };
int max_number_of_rows = 10;
char buffer[1024];



void start_text(){
	GetConsoleCursorInfo(hStdout, &lpCursor);
	lpCursor.bVisible = false;
	SetConsoleCursorInfo(hStdout, &lpCursor);
	
	coord.Y = 0;
}
void end_text(){
	coord.Y = 0;
}

//Improve this funciton as needed....
void print_text(const char* text, ...) {
	
	va_list argptr;
	va_start(argptr, text);
	vsprintf(buffer, text, argptr);
	va_end(argptr);

	int len = strlen(buffer);

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hStdout, coord);
	coord.Y++;
	WriteConsole(hStdout, "", 1, NULL, NULL);
	WriteConsole(hStdout, buffer, len, NULL, NULL);	
	
	//memset(&buffer[0], 0, sizeof(buffer));
}

char * FMOD_TypeToString(FMOD_SOUND_TYPE type)
{
	switch (type)
	{
	case FMOD_SOUND_TYPE_UNKNOWN: return "3rd party / unknown plugin format.";
	case FMOD_SOUND_TYPE_AIFF: return "AIFF.";
	case FMOD_SOUND_TYPE_ASF: return "Microsoft Advanced Systems Format (ie WMA/ASF/WMV).";
	case FMOD_SOUND_TYPE_DLS: return "Sound font / downloadable sound bank.";
	case FMOD_SOUND_TYPE_FLAC: return "FLAC lossless codec.";
	case FMOD_SOUND_TYPE_FSB: return "FMOD Sample Bank.";
	case FMOD_SOUND_TYPE_IT: return "Impulse Tracker.";
	case FMOD_SOUND_TYPE_MIDI: return "MIDI.";
	case FMOD_SOUND_TYPE_MOD: return "Protracker / Fasttracker MOD.";
	case FMOD_SOUND_TYPE_MPEG: return "MP2/MP3 MPEG.";
	case FMOD_SOUND_TYPE_OGGVORBIS: return "Ogg vorbis.";
	case FMOD_SOUND_TYPE_PLAYLIST: return "Information only from ASX/PLS/M3U/WAX playlists";
	case FMOD_SOUND_TYPE_RAW: return "Raw PCM data.";
	case FMOD_SOUND_TYPE_S3M: return "ScreamTracker 3.";
	case FMOD_SOUND_TYPE_USER: return "User created sound.";
	case FMOD_SOUND_TYPE_WAV: return "Microsoft WAV.";
	case FMOD_SOUND_TYPE_XM: return "FastTracker 2 XM.";
	case FMOD_SOUND_TYPE_XMA: return "Xbox360 XMA";
	case FMOD_SOUND_TYPE_AUDIOQUEUE: return "iPhone hardware decoder, supports AAC, ALAC and MP3.";
	case FMOD_SOUND_TYPE_AT9: return "PS4 / PSVita ATRAC 9 format";
	case FMOD_SOUND_TYPE_VORBIS: return "Vorbis";
	case FMOD_SOUND_TYPE_MEDIA_FOUNDATION: return "Windows Store Application built in system codecs";
	case FMOD_SOUND_TYPE_MEDIACODEC: return "Android MediaCodec";
	case FMOD_SOUND_TYPE_FADPCM: return "FMOD Adaptive Differential Pulse Code Modulation";
	case FMOD_SOUND_TYPE_MAX: return "Maximum number of sound types supported.";
	case FMOD_SOUND_TYPE_FORCEINT: return "Makes sure this enum is signed 32bit.";
	default:
		return "3rd party / unknown plugin format.";
		
	}
}

char * FMOD_FormatToString(FMOD_SOUND_FORMAT format)
{
	switch (format)
	{
	case FMOD_SOUND_FORMAT_NONE: return "Unitialized / unknown.";
	case FMOD_SOUND_FORMAT_PCM8: return "8bit integer PCM data.";
	case FMOD_SOUND_FORMAT_PCM16: return "16bit integer PCM data.";
	case FMOD_SOUND_FORMAT_PCM24: return "24bit integer PCM data.";
	case FMOD_SOUND_FORMAT_PCM32: return "32bit integer PCM data.";
	case FMOD_SOUND_FORMAT_PCMFLOAT: return "32bit floating point PCM data.";
	case FMOD_SOUND_FORMAT_BITSTREAM: return "Sound data is in its native compressed format.";
	case FMOD_SOUND_FORMAT_MAX: return "Maximum number of sound formats supported.";
	case FMOD_SOUND_FORMAT_FORCEINT: return "Makes sure this enum is signed 32bit.";
	default:
		return "Unitialized / unknown.";
	}
}



