/*
 * Project Luna
 * Copyright (C) 2014 David Jolly
 * ----------------------
 *
 * Luna is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Luna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdarg>
#include <stdexcept>
#include "luna.h"
#include "luna_trace_type.h"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

namespace LUNA_NS {

	void 
	trace_message(
		__in level_t level,
		__in const char *format,
		...
		)
	{
		size_t len;
		va_list args;
		char *buf = NULL;
		std::string message = LUNA_EXCEPTION_EXCEPTION_STRING(
			LUNA_TRACE_EXCEPTION_EMPTY_TRACE);

		if(!format || (level > TRACE_LEVEL_MAX)) {
			message = LUNA_EXCEPTION_EXCEPTION_STRING(
				LUNA_TRACE_EXCEPTION_INVALID_PARAMETER);
			goto exit;
		}

		va_start(args, format);
		if(!args) {
			message = LUNA_EXCEPTION_EXCEPTION_STRING(
				LUNA_TRACE_EXCEPTION_INTERNAL);
			goto exit;
		}

		len = _vscprintf(format, args) + EMPTY_STR_LEN;
		if(len == EMPTY_STR_LEN) {
			goto exit;
		}

		buf = new char[sizeof(char) * len];
		if(!buf) {
			message = LUNA_EXCEPTION_EXCEPTION_STRING(
				LUNA_TRACE_EXCEPTION_INTERNAL);
			goto exit;
		}
		
		if(vsprintf_s(buf, len, format, args) <= INVALID) {
			message = LUNA_EXCEPTION_EXCEPTION_STRING(
				LUNA_TRACE_EXCEPTION_INTERNAL);
			goto exit;
		}

		message.clear();
		message = buf;
		va_end(args);

exit:

		if(buf) {
			delete[] buf;
			buf = NULL;
		}

		if(level <= TRACE_LEVEL_SHOW) {

#ifdef _WIN32
			message += "\n";
			OutputDebugString(message.c_str());
#else
			if(level == TRACE_LEVEL_ERR) {
				std::cerr << message.c_str() << std::endl;
			} else {
				std::cout << message.c_str() << std::endl;
			}
#endif // _WIN32
		}
	}
}
