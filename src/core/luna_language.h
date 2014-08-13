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

#ifndef LUNA_LANGUAGE_H_
#define LUNA_LANGUAGE_H_

#include <set>

namespace LUNA_NS {

	class _luna;

	namespace LUNA_LANG_NS {
	
		#define CHARACTER_EOS '\0'
		#define SENTINEL_COUNT 2

		typedef enum {
			TOKEN_ARRAY_LIST = 0,
			TOKEN_ARITHMETIC_OPERATOR,
			TOKEN_ARRAY_INDEX,
			TOKEN_ASSIGNMENT,
			TOKEN_ASSIGNMENT_LIST,
			TOKEN_BEGIN,
			TOKEN_BINARY_OPERATOR,
			TOKEN_BOOLEAN,
			TOKEN_CONDITIONAL_LIST,
			TOKEN_CONTROL,
			TOKEN_END,
			TOKEN_EXPRESSION,
			TOKEN_EXPRESSION_COMPOUND,
			TOKEN_EXPRESSION_LIST,
			TOKEN_FLOAT,
			TOKEN_FUNCTION_CALL,
			TOKEN_IDENTIFIER,
			TOKEN_IDENTIFIER_LIST,
			TOKEN_KEYWORD,
			TOKEN_LITERAL_CHARACTER,
			TOKEN_LITERAL_STRING,
			TOKEN_OPERATOR,
			TOKEN_NULL,
			TOKEN_SCALAR,
			TOKEN_SHIFT_OPEATOR,
			TOKEN_STATEMENT,
			TOKEN_STATEMENT_LIST,
			TOKEN_SYMBOL,
		} token_t;

		#define TOKEN_MAX TOKEN_SYMBOL

		static const std::string TOKEN_STR[] = {
			"ARRAY_LIST", "ARITHMETIC_OPERATOR", "ARRAY_INDEX", "ASSIGNMENT", "ASSIGNMENT_LIST", "BEGIN", 
			"BINARY_OPERATOR", "BOOLEAN", "CONDITIONAL_LIST", "CONTROL", "END", "EXPRESSION", "EXPRESSION_COMPOUND", 
			"EXPRESSION_LIST", "FLOAT", "FUNCTION_CALL", "IDENTIFIER", "IDENTIFIER_LIST", "KEYWORD", "CHARACTER", 
			"STRING", "OPERATOR", "NULL", "SCALAR", "SHIFT_OPERATOR", "STATEMENT", "STATEMENT_LIST", "SYMBOL",
			};

		#define TOKEN_STRING(_TYPE_)\
			((_TYPE_) > TOKEN_MAX ? UNKNOWN : TOKEN_STR[_TYPE_])

		enum {
			ARITHMETIC_OPERATOR_ADDITION = 0,
			ARITHMETIC_OPERATOR_DIVISION,
			ARITHMETIC_OPERATOR_MODULUS,
			ARITHMETIC_OPERATOR_MULTIPLICATION,
			ARITHMETIC_OPERATOR_SUBTRACTION,
		};

		#define ARITHMETIC_OPERATOR_MAX ARITHMETIC_OPERATOR_SUBTRACTION

		static const std::string ARITHMETIC_OPERATOR_STR[] = {
			"+", "/", "%", "*", "-",
			};

		#define ARITHMETIC_OPERATOR_STRING(_TYPE_)\
			((_TYPE_) > ARITHMETIC_OPERATOR_MAX ? UNKNOWN : ARITHMETIC_OPERATOR_STR[_TYPE_])

		static const std::set<std::string> ARITHMETIC_OPERATOR_SET = {
			ARITHMETIC_OPERATOR_STR, ARITHMETIC_OPERATOR_STR + (ARITHMETIC_OPERATOR_MAX + 1)
			};

		#define IS_ARITHMETIC_OPERATOR(_STR_)\
			(ARITHMETIC_OPERATOR_SET.find(_STR_) != ARITHMETIC_OPERATOR_SET.end())

		enum {
			BINARY_OPERATOR_AND = 0,
			BINARY_OPERATOR_OR,
			BINARY_OPERATOR_XOR,
		};

		#define BINARY_OPERATOR_MAX BINARY_OPERATOR_XOR

		static const std::string BINARY_OPERATOR_STR[] = {
			"&", "|", "^",
			};

		#define BINARY_OPERATOR_STRING(_TYPE_)\
			((_TYPE_) > BINARY_OPERATOR_MAX ? UNKNOWN : BINARY_OPERATOR_STR[_TYPE_])

		static const std::set<std::string> BINARY_OPERATOR_SET = {
			BINARY_OPERATOR_STR, BINARY_OPERATOR_STR + (BINARY_OPERATOR_MAX + 1)
			};

		#define IS_BINARY_OPERATOR(_STR_) (BINARY_OPERATOR_SET.find(_STR_) != BINARY_OPERATOR_SET.end())

		enum {
			BOOLEAN_FALSE = 0,
			BOOLEAN_TRUE,
		};

		#define BOOLEAN_MAX BOOLEAN_TRUE

		static const std::string BOOLEAN_STR[] = {
			"false", "true",
			};

		#define BOOLEAN_STRING(_TYPE_)\
			((_TYPE_) > BOOLEAN_MAX ? UNKNOWN : BOOLEAN_STR[_TYPE_])

		static const std::set<std::string> BOOLEAN_SET = {
			BOOLEAN_STR, BOOLEAN_STR + (BOOLEAN_MAX + 1)
			};

		#define IS_BOOLEAN(_STR_) (BOOLEAN_SET.find(_STR_) != BOOLEAN_SET.end())

		enum {
			CONTROL_BREAK = 0,
			CONTROL_CONTINUE,
		};

		#define CONTROL_MAX CONTROL_CONTINUE

		static const std::string CONTROL_STR[] = {
			"break", "continue", 
			};

		#define CONTROL_STRING(_TYPE_)\
			((_TYPE_) > CONTROL_MAX ? UNKNOWN : CONTROL_STR[_TYPE_])

		static const std::set<std::string> CONTROL_SET = {
			CONTROL_STR, CONTROL_STR + (CONTROL_MAX + 1)
			};

		#define IS_CONTROL(_STR_) (CONTROL_SET.find(_STR_) != CONTROL_SET.end())

		enum {
			KEYWORD_ELSE,
			KEYWORD_FUNCT,
			KEYWORD_IF,
			KEYWORD_PRINT,
			KEYWORD_STRUCT,
			KEYWORD_WHILE,
		};

		#define KEYWORD_MAX KEYWORD_WHILE

		static const std::string KEYWORD_STR[] = {
			"else", "funct", "if", "print", "struct", "while",
			};

		#define KEYWORD_STRING(_TYPE_)\
			((_TYPE_) > KEYWORD_MAX ? UNKNOWN : KEYWORD_STR[_TYPE_])

		static const std::set<std::string> KEYWORD_SET = {
			KEYWORD_STR, KEYWORD_STR + (KEYWORD_MAX + 1)
			};

		#define IS_KEYWORD(_STR_) (KEYWORD_SET.find(_STR_) != KEYWORD_SET.end())

		enum {
			OPERATOR_AND = 0,
			OPERATOR_EQUALS,
			OPERATOR_GREATER_THAN,
			OPERATOR_GREATER_THAN_EQUALS,
			OPERATOR_LESS_THAN,
			OPERATOR_LESS_THAN_EQUALS,
			OPERATOR_NOT_EQUALS,
			OPERATOR_OR,
			OPERATOR_SHIFT_LEFT,
			OPERATOR_SHIFT_RIGHT,
		};

		#define OPERATOR_MAX OPERATOR_SHIFT_RIGHT

		static const std::string OPERATOR_STR[] = {
			"&&", "==", ">", ">=", "<", "<=", "~=", "||",
			};

		#define OPERATOR_STRING(_TYPE_)\
			((_TYPE_) > OPERATOR_MAX ? UNKNOWN : OPERATOR_STR[_TYPE_])

		static const std::set<std::string> OPERATOR_SET = {
			OPERATOR_STR, OPERATOR_STR + (OPERATOR_MAX + 1)
			};

		#define IS_OPERATOR(_STR_) (OPERATOR_SET.find(_STR_) != OPERATOR_SET.end())

		enum {
			NULL_TYPE = 0,
		};

		#define NULL_MAX NULL_TYPE

		static const std::string NULL_STR[] = {
			"null",
			};

		#define NULL_STRING(_TYPE_)\
			((_TYPE_) > NULL_MAX ? UNKNOWN : NULL_STR[_TYPE_])

		static const std::set<std::string> NULL_SET = {
			NULL_STR, NULL_STR + (NULL_MAX + 1)
			};

		#define IS_NULL(_STR_) (NULL_SET.find(_STR_) != NULL_SET.end())

		enum {
			SCALAR_BINARY = 0,
			SCALAR_DECIMAL,
			SCALAR_HEXIDECIMAL,
			SCALAR_OCTAL,
		};

		#define SCALAR_MAX SCALAR_OCTAL

		static const std::string SCALAR_STR[] = {
			"BINARY", "DECIMAL", "HEXIDECIMAL", "OCTAL",
			};

		#define SCALAR_STRING(_TYPE_)\
			((_TYPE_) > SCALAR_MAX ? UNKNOWN : SCALAR_STR[_TYPE_])

		static const std::set<std::string> SCALAR_SET = {
			SCALAR_STR, SCALAR_STR + (SCALAR_MAX + 1)
			};

		#define IS_SCALAR(_STR_) (SCALAR_SET.find(_STR_) != SCALAR_SET.end())

		enum {
			SHIFT_OPERATOR_LEFT = 0,
			SHIFT_OPERATOR_RIGHT,
		};

		#define SHIFT_OPERATOR_MAX SHIFT_OPERATOR_RIGHT

		static const std::string SHIFT_OPERATOR_STR[] = {
			"<<", ">>",
			};

		#define SHIFT_OPERATOR_STRING(_TYPE_)\
			((_TYPE_) > SHIFT_OPERATOR_MAX ? UNKNOWN : SHIFT_OPERATOR_STR[_TYPE_])

		static std::set<std::string> SHIFT_OPERATOR_SET = {
			SHIFT_OPERATOR_STR, SHIFT_OPERATOR_STR + (SHIFT_OPERATOR_MAX + 1)
			};

		#define IS_SHIFT_OPERATOR(_STR_) (SHIFT_OPERATOR_SET.find(_STR_) != SHIFT_OPERATOR_SET.end())

		enum {
			SYMBOL_ASSIGNMENT = 0,
			SYMBOL_CURLY_BRACE_CLOSE,
			SYMBOL_CURLY_BRACE_OPEN,
			SYMBOL_NEGATIVE,
			SYMBOL_PARENTHESIS_CLOSE,
			SYMBOL_PARENTHESIS_OPEN,
			SYMBOL_REFERENCE,
			SYMBOL_SEPERATOR,
			SYMBOL_SQUARE_BRACKET_CLOSE,
			SYMBOL_SQUARE_BRACKET_OPEN,
			SYMBOL_TERMINATOR,
		};

		#define SYMBOL_MAX SYMBOL_TERMINATOR

		static const std::string SYMBOL_STR[] = {
			"=", "}", "{", "~", ")", "(", ".", ",", "]", "[", ";",
			};

		#define SYMBOL_STRING(_TYPE_)\
			((_TYPE_) > SYMBOL_MAX ? UNKNOWN : SYMBOL_STR[_TYPE_])

		static const std::set<std::string> SYMBOL_SET = {
			SYMBOL_STR, SYMBOL_STR + (SYMBOL_MAX + 1)
			};

		#define IS_SYMBOL(_STR_) (SYMBOL_SET.find(_STR_) != SYMBOL_SET.end())

		static const std::string *TOKEN_SUBTYPE_STRS[] = {
			NULL, ARITHMETIC_OPERATOR_STR, NULL, NULL, NULL, NULL, BINARY_OPERATOR_STR, BOOLEAN_STR, 
			NULL, CONTROL_STR, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, KEYWORD_STR, NULL, NULL, 
			OPERATOR_STR, NULL_STR, SCALAR_STR, SHIFT_OPERATOR_STR, NULL, NULL, SYMBOL_STR,
			};

		static const size_t TOKEN_SUBTYPE_STRS_LEN[] = {
			0, (ARITHMETIC_OPERATOR_MAX + 1), 0, 0, 0, 0, (BINARY_OPERATOR_MAX + 1), (BOOLEAN_MAX + 1), 
			0, (CONTROL_MAX + 1), 0, 0, 0, 0, 0, 0, 0, 0, (KEYWORD_MAX + 1), 0, 0, (OPERATOR_MAX + 1), 
			(NULL_MAX + 1), (SCALAR_MAX + 1), (SHIFT_OPERATOR_MAX + 1), 0, 0, (SYMBOL_MAX + 1),
			};

		#define TOKEN_SUBTYPE_STRINGS(_TYPE_)\
			((_TYPE_) > TOKEN_MAX ? NULL : TOKEN_SUBTYPE_STRS[_TYPE_])
		#define TOKEN_SUBTYPE_STRINGS_LENGTH(_TYPE_)\
			((_TYPE_) > TOKEN_MAX ? 0 : TOKEN_SUBTYPE_STRS_LEN[_TYPE_])

		uint32_t determine_token_subtype(
			__in const std::string &text,
			__in token_t type
			);

		typedef class _luna_lang_base {
		
			public:

				_luna_lang_base(void);

				_luna_lang_base(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				_luna_lang_base(
					__in const _luna_lang_base &other
					);

				virtual ~_luna_lang_base(void);

				_luna_lang_base &operator=(
					__in const _luna_lang_base &other
					);

				virtual void clear(void);

				virtual size_t discover(void) = 0;

				_luna *get_context(void);

				std::string &get_source(void);

				std::string &get_text(void);

				bool is_file(void);

				virtual void reset(void) = 0;

				virtual size_t size(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				void acquire_context(void);

				_luna *m_context;

				std::string m_source;

				std::string m_text;

			private:

				std::recursive_mutex m_lock;

		} luna_lang_base, *luna_lang_base_ptr;
	}
}

#endif // LUNA_LANGUAGE_H_
