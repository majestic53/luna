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

#ifndef LUNA_EXCEPTION_H_
#define LUNA_EXCEPTION_H_

namespace LUNA_NS {

	#define UNKNOWN_EXCEPTION "Unknown exception"

	void throw_exception(
		__in const char *format,
		...
		);

#ifdef _DEBUG
	#define THROW_EXCEPTION(_EXCEPT_)\
		throw_exception("(%s(%s)) %s", __FUNCTION__, CONCAT_STR(__LINE__), _EXCEPT_)
	#define THROW_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
		throw_exception("(%s(%s)) %s: " _FORMAT_, __FUNCTION__, CONCAT_STR(__LINE__),\
			_EXCEPT_, __VA_ARGS__)
#else
	#define THROW_EXCEPTION(_EXCEPT_)\
		throw_exception("%s", _EXCEPT_)
	#define THROW_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
		throw_exception("%s: " _FORMAT_, _EXCEPT_, __VA_ARGS__)
#endif // _DEBUG
}

#endif // LUNA_EXCEPTION_H_
