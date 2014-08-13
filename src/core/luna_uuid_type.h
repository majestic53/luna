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

#ifndef LUNA_UUID_TYPE_H_
#define LUNA_UUID_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		enum {
			LUNA_UUID_EXCEPTION_ACQUIRY_FAILED = 0,
			LUNA_UUID_EXCEPTION_ALLOC_FAILED,
			LUNA_UUID_EXCEPTION_ID_NOT_FOUND,
			LUNA_UUID_EXCEPTION_LIBARY_ACQUIRY_FAILED,
			LUNA_UUID_EXCEPTION_UNINITIALIZED,
			LUNA_UUID_EXCEPTION_UNIQUE_ID_REFERENCED,
		};

		static const std::string LUNA_UUID_EXCEPTION_STR[] = {
			"Failed to acquire UUID factory",
			"UUID factory allocation failed",
			"UUID not found",
			"Failed to acquire library",
			"UUID factory is uninitialized",
			"Unique UUID referenced multiple times",
			};

		#define LUNA_UUID_EXCEPTION_MAX LUNA_UUID_EXCEPTION_UNIQUE_ID_REFERENCED

		#define LUNA_UUID_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_UUID_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : LUNA_UUID_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_UUID_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_UUID_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_UUID_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_UUID_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _uuid;
		typedef _uuid uuid, *uuid_ptr;

		class _uuid_factory;
		typedef _uuid_factory uuid_factory, *uuid_factory_ptr;

		class _uuid_base;
		typedef _uuid_base uuid_base, *uuid_base_ptr;
	}
}

#endif // LUNA_UUID_TYPE_H_
