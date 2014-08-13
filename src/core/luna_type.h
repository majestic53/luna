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

#ifndef LUNA_TYPE_H_
#define LUNA_TYPE_H_

namespace LUNA_NS {
	
	enum {
		LUNA_EXCEPTION_ALLOC_FAILED = 0,
		LUNA_EXCEPTION_FACTORY_ALLOC_FAILED,
		LUNA_EXCEPTION_FACTORY_UNINITIALIZED,
		LUNA_EXCEPTION_UNINITIALIZED,
	};

	static const std::string LUNA_EXCEPTION_STR[] = {
		"Library allocation failed",
		"Library factory allocation failed",
		"Library factory is uninitialized",
		"Library is uninitialized",
		};

	#define LUNA_EXCEPTION_MAX LUNA_EXCEPTION_UNINITIALIZED

	#define LUNA_EXCEPTION_STRING(_EXCEPT_)\
		((_EXCEPT_) > LUNA_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : LUNA_EXCEPTION_STR[_EXCEPT_].c_str())
	#define THROW_LUNA_EXCEPTION(_EXCEPT_)\
		THROW_EXCEPTION(LUNA_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_LUNA_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
		THROW_EXCEPTION_MESSAGE(LUNA_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

	class _luna;
	typedef _luna luna, *luna_ptr;
}

#endif // LUNA_TYPE_H_
