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

#ifndef LUNA_TOKEN_TYPE_H_
#define LUNA_TOKEN_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		enum {
			LUNA_TOKEN_EXCEPTION_ALLOC_FAILED = 0,
			LUNA_TOKEN_EXCEPTION_ID_ALREADY_ADDED,
			LUNA_TOKEN_EXCEPTION_ID_NOT_FOUND,
			LUNA_TOKEN_EXCEPTION_UNINITIALIZED,
		};

		static const std::string LUNA_TOKEN_EXCEPTION_STR[] = {
			"Token factory allocation failed",
			"Token UUID was already added",
			"Token UUID was not found",
			"Token factory is uninitialized",
			};

		#define LUNA_TOKEN_EXCEPTION_MAX LUNA_TOKEN_EXCEPTION_UNINITIALIZED

		#define LUNA_TOKEN_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_TOKEN_EXCEPTION_MAX ? UNKNOWN_EXCEPTION :LUNA_TOKEN_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_TOKEN_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_TOKEN_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_TOKEN_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_TOKEN_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _token;
		typedef _token token, *token_ptr;

		class _token_factory;
		typedef _token_factory token_factory, *token_factory_ptr;
	}
}

#endif // LUNA_TOKEN_TYPE_H_
