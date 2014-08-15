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

#ifndef LUNA_SCOPE_TYPE_H_
#define LUNA_SCOPE_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_COMP_NS {
		
		enum {
			LUNA_SCOPE_EXCEPTION_ALLOC_FAILED = 0,
			LUNA_SCOPE_EXCEPTION_ID_NOT_FOUND,
			LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
			LUNA_SCOPE_EXCEPTION_INVALID_KEY,
			LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL,
			LUNA_SCOPE_EXCEPTION_INVALID_TYPE,
			LUNA_SCOPE_EXCEPTION_KEY_ALREADY_EXISTS,
			LUNA_SCOPE_EXCEPTION_KEY_NOT_FOUND,
			LUNA_SCOPE_EXCEPTION_SCOPE_NOT_FOUND,
			LUNA_SCOPE_EXCEPTION_UNINITIALIZED,
		};

		static const std::string LUNA_SCOPE_EXCEPTION_STR[] = {
			"Scope factory allocation failed",
			"Symbol id not found in scope",
			"Invalid symbol index",
			"Invalid symbol key",
			"Symbol is not allocated",
			"Invalid symbol type",
			"Symbol already exists in scope",
			"Symbol was not found in scope",
			"Scope was not found",
			"Scope factory uninitialized",
			};

		#define LUNA_SCOPE_EXCEPTION_MAX LUNA_SCOPE_EXCEPTION_UNINITIALIZED

		#define LUNA_SCOPE_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_SCOPE_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : LUNA_SCOPE_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_SCOPE_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_SCOPE_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _symbol;
		typedef _symbol symbol, *symbol_ptr;

		class _symbol_array;
		typedef _symbol_array symbol_array, *symbol_array_ptr;

		class _symbol_boolean;
		typedef _symbol_boolean symbol_boolean, *symbol_boolean_ptr;

		class _symbol_float;
		typedef _symbol_float symbol_float, *symbol_float_ptr;

		class _symbol_function;
		typedef _symbol_function symbol_function, *symbol_function_ptr;

		class _symbol_identifier;
		typedef _symbol_identifier symbol_identifier, *symbol_identifier_ptr;

		class _symbol_integer;
		typedef _symbol_integer symbol_integer, *symbol_integer_ptr;

		class _symbol_literal_string;
		typedef _symbol_literal_string symbol_literal_string, *symbol_literal_string_ptr;

		class _symbol_null;
		typedef _symbol_null symbol_null, *symbol_null_ptr;

		class _symbol_structure;
		typedef _symbol_structure symbol_structure, *symbol_structure_ptr;

		class _scope;
		typedef _scope scope, *scope_ptr;

		class _scope_factory;
		typedef _scope_factory scope_factory, *scope_factory_ptr;
	}
}

#endif // LUNA_SCOPE_TYPE_H_
