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

#include "luna.h"

namespace LUNA_NS {

#ifndef _WIN32
	size_t 
	_vscprintf(
		__in const char *format,
		__in va_list arguments
		)
	{
		return vsnprintf(NULL, 0, format, arguments);
	}

	size_t 
	vsprintf_s(
		__out char *buffer,
		__in size_t length,
		__in const char *format,
		__in va_list arguments
		)
	{
		size_t result;

		if(!buffer || !length || !format) {
			result = INVALID;
			errno = EINVAL;
			goto exit;
		}

		result = vsnprintf(buffer, length, format, arguments);

exit:
		return result;
	}
#endif // _WIN32
}
