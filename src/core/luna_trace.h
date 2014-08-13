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

#ifndef LUNA_TRACE_H_
#define LUNA_TRACE_H_

namespace LUNA_NS {

	typedef enum {
		TRACE_LEVEL_ERR = 0,
		TRACE_LEVEL_INFO,
		TRACE_LEVEL_VERB,
	} level_t, *level_ptr_t;

	#define TRACE_LEVEL_MAX TRACE_LEVEL_VERB
	#define TRACE_LEVEL_SHOW TRACE_LEVEL_VERB

	void trace_message(
		__in level_t level,
		__in const char *format,
		...
		);

#ifdef _DEBUG
	#define TRACE_MESSAGE(_LEVEL_, _FORMAT_, ...)\
		trace_message(_LEVEL_, _FORMAT_, __VA_ARGS__)
#else
	#define TRACE_MESSAGE(_LEVEL_, _FORMAT_, ...)
#endif // _DEBUG
	#define TRACE_ERR(_FORMAT_, ...)\
		TRACE_MESSAGE(TRACE_LEVEL_ERR, "%s: " _FORMAT_, FNAME, __VA_ARGS__)
	#define TRACE_INFO(_FORMAT_, ...)\
		TRACE_MESSAGE(TRACE_LEVEL_INFO, "%s: " _FORMAT_, FNAME, __VA_ARGS__)
	#define TRACE_VERB(_FORMAT_, ...)\
		TRACE_MESSAGE(TRACE_LEVEL_VERB, "%s: " _FORMAT_, FNAME, __VA_ARGS__)
	#define TRACE_ENTRY() TRACE_MESSAGE(TRACE_LEVEL_VERB, "%s", FENTRY)
	#define TRACE_EXIT(_FORMAT_, ...)\
		TRACE_MESSAGE(TRACE_LEVEL_VERB, "%s: " _FORMAT_, FEXIT, __VA_ARGS__)
}

#endif // LUNA_TRACE_H_
