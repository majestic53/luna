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

#ifndef LUNA_DEFINES_H_
#define LUNA_DEFINES_H_

#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

#ifndef _WIN32
#define __in
#define __in_opt
#define __out
#define __out_opt
#define __inout
#define __inout_opt
#endif // _WIN32

namespace LUNA_NS {

	#define BLOCK_SIZE 0x10
	#define EMPTY_STR_LEN 1
	#define INIT_REF 1
	#define INVALID (-1)
	#define NONE "NONE"
	#define UNKNOWN "UNKNOWN"

#ifdef _DEBUG
	#define FNAME __FUNCTION__ "(" CONCAT_STR(__LINE__) ")"
#else
	#define FNAME __FUNCTION__
#endif // _DEBUG
	#define FENTRY "+" FNAME
	#define FEXIT "-" FNAME

	#define _CONCAT_STR(_STR_) #_STR_
	#define CONCAT_STR(_STR_) _CONCAT_STR(_STR_)

	#define _SERIALIZE_CALL(_TYPE_, _MUTEX_) std::lock_guard<_TYPE_> __LOCK(_MUTEX_)
	#define SERIALIZE_CALL(_MUTEX_) _SERIALIZE_CALL(std::mutex, _MUTEX_)
	#define SERIALIZE_CALL_RECUR(_MUTEX_) _SERIALIZE_CALL(std::recursive_mutex, _MUTEX_)

	#define TRANSFORM(_FUNCT_, _VECTOR_)\
		std::transform((_VECTOR_).begin(), (_VECTOR_).end(), (_VECTOR_).begin(), _FUNCT_)

	#define UNREF_PARAM(_PARAM_) (void) _PARAM_

	#define VALUE_AS_HEX(_TYPE_, _VALUE_)\
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex\
			<< (uintmax_t) ((_TYPE_) (_VALUE_)) << std::dec

	template <typename FUNCT, typename TYPE> TYPE 
	transform_inline(
		__in const FUNCT &funct,
		__in const TYPE &input
		)
	{
		TYPE output = input;

		TRANSFORM(funct, output);

		return output;
	}

	#define TO_LOWER(_STR_) transform_inline(tolower, _STR_)

	#define TO_LOWER_CHAR(_CH_)\
		transform_inline(tolower, std::string(1, _CH_)).c_str()[0]

	typedef void(*generic_cb_t)(void);

#ifndef _WIN32
	size_t 
	_vscprintf(
		__in const char *format,
		__in va_list arguments
		);

	size_t 
	vsprintf_s(
		__out char *buffer,
		__in size_t length,
		__in const char *format,
		__in va_list arguments
		);
#endif
}

#endif // LUNA_DEFINES_H_
