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

#ifndef LUNA_LEXER_TYPE_H_
#define LUNA_LEXER_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_LANG_NS {
	
		enum {
			LUNA_LEXER_EXCEPTION_EXPECTING_ALPHA_CHARACTER = 0,
			LUNA_LEXER_EXCEPTION_EXPECTING_CHARACTER_DELIMITER,
			LUNA_LEXER_EXCEPTION_EXPECTING_CHARACTER_LITERAL,
			LUNA_LEXER_EXCEPTION_EXPECTING_DIGIT_CHARACTER,
			LUNA_LEXER_EXCEPTION_EXPECTING_NEXT_CHARACTER,
			LUNA_LEXER_EXCEPTION_EXPECTING_NEXT_TOKEN,
			LUNA_LEXER_EXCEPTION_EXPECTING_PREVIOUS_CHARACTER,
			LUNA_LEXER_EXCEPTION_EXPECTING_PREVIOUS_TOKEN,
			LUNA_LEXER_EXCEPTION_EXPECTING_STRING_DELIMITER,
			LUNA_LEXER_EXCEPTION_EXPECTING_SYMBOL_CHARACTER,
			LUNA_LEXER_EXCEPTION_FACTORY_ALLOC_FAILED,
			LUNA_LEXER_EXCEPTION_INVALID_CHARACTER_POSITION,
			LUNA_LEXER_EXCEPTION_INVALID_CHARACTER_ROW,
			LUNA_LEXER_EXCEPTION_INVALID_SCALAR_VALUE,
			LUNA_LEXER_EXCEPTION_INVALID_SCALAR_FLOAT_VALUE,
			LUNA_LEXER_EXCEPTION_INVALID_TOKEN_POSITION,
			LUNA_LEXER_EXCEPTION_NO_NEXT_CHARACTER,
			LUNA_LEXER_EXCEPTION_NO_NEXT_TOKEN,
			LUNA_LEXER_EXCEPTION_NO_PREVIOUS_CHARACTER,
			LUNA_LEXER_EXCEPTION_NO_PREVIOUS_TOKEN,
			LUNA_LEXER_EXCEPTION_SCALAR_PARSE_FAILED,
			LUNA_LEXER_EXCEPTION_UNKNOWN_SCALAR_TYPE,
			LUNA_LEXER_EXCEPTION_UNKNOWN_SYMBOL_CHARACTER,
			LUNA_LEXER_EXCEPTION_UNTERMINATED_CHARACTER_LITERAL,
			LUNA_LEXER_EXCEPTION_UNTERMINATED_ESCAPE_SEQUENCE,
			LUNA_LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL,
		};

		static const std::string LUNA_LEXER_EXCEPTION_STR[] = {
			"Expecting alpha character type",
			"Expecting character delimiter",
			"Expecting character literal",
			"Expecting digit character type",
			"Expecting a next character",
			"Expecting a next token",
			"Expecting a previous character",
			"Expecting a previous token",
			"Expecting string delimiter",
			"Expecting symbol character type",
			"Factory allocation failed",
			"Invalid character position",
			"Invalid character row",
			"Invalid scalar value",
			"Invalid floating-point value",
			"Invalid token position",
			"No next character found",
			"No next token found",
			"No previous character found",
			"No previous token found",
			"Failed to parse scalar value",
			"Unknown scalar type",
			"Unknown symbol character",
			"Unterminated character literal",
			"Unterminated escape sequence",
			"Unterminated string literal",
			};

		#define LUNA_LEXER_EXCEPTION_MAX LUNA_LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL

		#define LUNA_LEXER_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_LEXER_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : LUNA_LEXER_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_LEXER_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_LEXER_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_LEXER_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _lexer_base;
		typedef _lexer_base lexer_base, *lexer_base_ptr;

		class _lexer;
		typedef _lexer lexer, *lexer_ptr;
	}
}

#endif // LUNA_LEXER_TYPE_H_
