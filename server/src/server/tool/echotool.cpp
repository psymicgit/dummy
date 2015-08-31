///<------------------------------------------------------------------------------
//< @file:   server\tool\echotool.cpp
//< @author: 洪坤安
//< @date:   2015年4月9日 11:10:41
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "echotool.h"

#ifdef WIN
	#include <windows.h>
	#include <io.h>
#else
	#include <stdarg.h>
#endif

/* 下面颜色改变的代码摘自gtest，只稍微做了一些调整 */
#ifdef WIN

namespace posix
{
	inline int isAtty(int fd) { return _isatty(fd); }
	inline int getFileNo(FILE* file) { return _fileno(file); }
}


// Returns the character attribute for the given color.
uint16 getColorAttribute(ECHO_COLOR color)
{
	switch (color) {
	case COLOR_RED:
		return FOREGROUND_RED | FOREGROUND_INTENSITY;
	case COLOR_GREEN:
		return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	case COLOR_YELLOW:
		return FOREGROUND_RED | FOREGROUND_GREEN;
	default:
		return 0;
	}
}
#else
// Returns the ANSI color code for the given color.  COLOR_DEFAULT is
// an invalid input.
const char* getAnsiColorCode(ECHO_COLOR color)
{
	switch (color) {
	case COLOR_RED:
		return "1";
	case COLOR_GREEN:
		return "2";
	case COLOR_YELLOW:
		return "3";
	default:
		return NULL;
	};
}
#endif

#ifdef WIN
// Helpers for printing colored strings to stdout. Note that on Windows, we
// cannot simply emit special characters and have the terminal change colors.
// This routine must actually emit the characters rather than return a string
// that would be colored when printed, as can be done on Linux.
void ColoredPrintf(ECHO_COLOR color, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	echotool::changeToColor(color);
	vprintf(fmt, args);
	echotool::changeToOldColor();
	va_end(args);
}
#endif

namespace echotool
{
	thread_local uint16 old_color_attrs = 0;

	void changeToColor(ECHO_COLOR color)
	{
#ifdef WIN
		static const bool in_color_mode = (posix::isAtty(posix::getFileNo(stdout)) > 0);
		const bool use_color = in_color_mode && (color != COLOR_DEFAULT);

		if (!use_color) {
			return;
		}

		const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		// Gets the current text color.
		CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
		old_color_attrs = buffer_info.wAttributes;

		// We need to flush the stream buffers into the console before each
		// SetConsoleTextAttribute call lest it affect the text that is already
		// printed but has not yet reached the console.
		fflush(stdout);
		SetConsoleTextAttribute(stdout_handle, getColorAttribute(color));// | FOREGROUND_INTENSITY);
#else
		printf("\033[0;3%sm", getAnsiColorCode(color));
#endif
	}

	void changeToOldColor()
	{
#ifdef WIN
		const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		fflush(stdout);
		SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
		printf("\033[m");  // Resets the terminal to default.
#endif
	}

	const char* getmsg(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		vsprintf_s(global::g_sprintfBuf, sizeof(global::g_sprintfBuf), fmt, args);

		va_end(args);
		return global::g_sprintfBuf;
	}
}

