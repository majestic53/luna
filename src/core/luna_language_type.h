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

#ifndef LUNA_LANGUAGE_TYPE_H_
#define LUNA_LANGUAGE_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_LANG_NS {
		
		enum {
			LUNA_LANGUAGE_EXCEPTION_CONTEXT_ALLOC_FAILED = 0,
			LUNA_LANGUAGE_EXCEPTION_FILE_NOT_FOUND,
		};

		static const std::string LUNA_LANGUAGE_EXCEPTION_STR[] = {
			"Context allocation failed",
			"File was not found",
			};

		#define LUNA_LANGUAGE_EXCEPTION_MAX LUNA_LANGUAGE_EXCEPTION_FILE_NOT_FOUND

		#define LUNA_LANGUAGE_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_LANGUAGE_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : \
				LUNA_LANGUAGE_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_LANGUAGE_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_LANGUAGE_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_LANGUAGE_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_LANGUAGE_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _luna_lang_base;
		typedef _luna_lang_base luna_lang_base, *luna_lang_base_ptr;
	}
}

#endif // LUNA_LANGUAGE_TYPE_H_
