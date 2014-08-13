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

#ifndef LUNA_TRACE_TYPE_H_
#define LUNA_TRACE_TYPE_H_

namespace LUNA_NS {

	enum {
		LUNA_TRACE_EXCEPTION_EMPTY_TRACE = 0,
		LUNA_TRACE_EXCEPTION_INTERNAL,
		LUNA_TRACE_EXCEPTION_INVALID_PARAMETER,
	};

	static const std::string LUNA_TRACE_EXCEPTION_STR[] = {
		"Empty exception",
		"Internal tracing error",
		"Invalid tracing parameter",
		};

	#define LUNA_TRACE_EXCEPTION_MAX LUNA_TRACE_EXCEPTION_INVALID_PARAMETER

	#define LUNA_EXCEPTION_EXCEPTION_STRING(_EXCEPT_)\
		((_EXCEPT_) > LUNA_TRACE_EXCEPTION_MAX ? UNKNOWN_EXCEPTION\
			: LUNA_TRACE_EXCEPTION_STR[_EXCEPT_].c_str())
}

#endif // LUNA_TRACE_TYPE_H_
