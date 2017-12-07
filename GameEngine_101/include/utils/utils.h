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
#pragma once

#include <stdio.h>
#include <Windows.h>

#include <fmod\fmod.hpp>
#include <fmod\fmod_errors.h>

extern HANDLE hStdout;
extern CONSOLE_CURSOR_INFO lpCursor;
extern COORD coord;
extern int max_number_of_rows;
extern char buffer[];


void start_text();
void end_text();
void print_text(const char* text, ...);

char *FMOD_TypeToString(FMOD_SOUND_TYPE type);
char *FMOD_FormatToString(FMOD_SOUND_FORMAT format);