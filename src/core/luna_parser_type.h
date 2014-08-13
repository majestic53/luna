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

#ifndef LUNA_PARSER_TYPE_H_
#define LUNA_PARSER_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_LANG_NS {
	
		enum {
			LUNA_PARSER_EXCEPTION_EXPECTING_ASSIGNMENT = 0,
			LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
			LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACKET,
			LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
			LUNA_PARSER_EXCEPTION_EXPECTING_CONDITIONAL_EXPRESSION,
			LUNA_PARSER_EXCEPTION_EXPECTING_CONDITIONAL_STATEMENT,
			LUNA_PARSER_EXCEPTION_EXPECTING_CONTROL,
			LUNA_PARSER_EXCEPTION_EXPECTING_ELSE_CONDITIONAL,
			LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
			LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION_NUMERIC,
			LUNA_PARSER_EXCEPTION_EXPECTING_FUNCTION,
			LUNA_PARSER_EXCEPTION_EXPECTING_GENERIC_CALL,
			LUNA_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
			LUNA_PARSER_EXCEPTION_EXPECTING_IF_CONDITIONAL,
			LUNA_PARSER_EXCEPTION_EXPECTING_KEYWORD,
			LUNA_PARSER_EXCEPTION_EXPECTING_NEXT_STATEMENT,
			LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
			LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_PARENTHESIS,
			LUNA_PARSER_EXCEPTION_EXPECTING_PREVIOUS_STATEMENT,
			LUNA_PARSER_EXCEPTION_EXPECTING_SCALAR,
			LUNA_PARSER_EXCEPTION_EXPECTING_STATEMENT,
			LUNA_PARSER_EXCEPTION_EXPECTING_STRING,
			LUNA_PARSER_EXCEPTION_EXPECTING_STRUCT,
			LUNA_PARSER_EXCEPTION_EXPECTING_TERMINATOR,
			LUNA_PARSER_EXCEPTION_FACTORY_ALLOC_FAILED,
			LUNA_PARSER_EXCEPTION_INVALID_CHILD_STATEMENT_POSITION,
			LUNA_PARSER_EXCEPTION_INVALID_PARENT_STATEMENT_POSITION,
			LUNA_PARSER_EXCEPTION_INVALID_STATEMENT_POSITION,
			LUNA_PARSER_EXCEPTION_NO_NEXT_STATEMENT,
			LUNA_PARSER_EXCEPTION_NO_PREVIOUS_STATEMENT,
		};

		static const std::string LUNA_PARSER_EXCEPTION_STR[] = {
			"Expecting an assignment",
			"Expecting a closing bracket",
			"Expecting a closing brace",
			"Expecting a closing parenthesis",
			"Expecting a conditional expression",
			"Expecting a conditional statement",
			"Expecting a control expression",
			"Expecting an else condition",
			"Expecting an expression",
			"Expecting a numeric expression",
			"Expecting a function",
			"Expecting a generic call",
			"Expecting an identifier",
			"Expecting an if condition",
			"Expecting a keyword",
			"Expecting a next statement",
			"Expecting an opening brace",
			"Expecting an opening parenthesis",
			"Expecting a previous statement",
			"Expecting a scalar value",
			"Expecting a statement",
			"Expecting a string",
			"Expecting a structure",
			"Expecting a terminator",
			"Factory allocation failed",
			"Invalid child statement position",
			"Invalid parent statement position",
			"Invalid statement position",
			"No next statement found",
			"No previous statement found",
			};

		#define LUNA_PARSER_EXCEPTION_MAX LUNA_PARSER_EXCEPTION_NO_PREVIOUS_STATEMENT

		#define LUNA_PARSER_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_PARSER_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : LUNA_PARSER_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_PARSER_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_PARSER_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_PARSER_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _parser;
		typedef _parser parser, *parser_ptr;
	}
}

#endif // LUNA_PARSER_TYPE_H_
