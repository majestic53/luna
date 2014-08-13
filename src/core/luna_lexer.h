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

#ifndef LUNA_LEXER_H_
#define LUNA_LEXER_H_

namespace LUNA_NS {

	using namespace LUNA_COMP_NS;

	namespace LUNA_LANG_NS {

		typedef enum {
			CHARACTER_ALPHA = 0,
			CHARACTER_DIGIT,
			CHARACTER_SYMBOL,
			CHARACTER_WHITESPACE,
		} char_t;

		#define CHARACTER_MAX CHARACTER_WHITESPACE

		typedef class _lexer_base :
				protected luna_lang_base {
		
			public:

				_lexer_base(void);

				_lexer_base(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				_lexer_base(
					__in const _lexer_base &other
					);

				virtual ~_lexer_base(void);

				_lexer_base &operator=(
					__in const _lexer_base &other
					);

				virtual void clear(void);

				virtual size_t discover(void);

				char get_character(void);

				char get_character(
					__in size_t position
					);

				size_t get_character_column(void);

				size_t get_character_position(void);

				size_t get_character_row(void);

				char_t get_character_type(void);

				bool has_next_character(void);

				bool has_previous_character(void);

				char move_next_character(
					__in_opt bool expect_next = false
					);

				char move_previous_character(
					__in_opt bool expect_previous = false
					);

				virtual void reset(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				void determine_character_type(void);

				char_t determine_character_type(
					__in size_t position
					);

				size_t m_char_column;

				std::map<size_t, size_t> m_char_column_map;

				size_t m_char_position;

				size_t m_char_row;

				char_t m_char_type;

			private:

				std::recursive_mutex m_lock;

		} lexer_base, *lexer_base_ptr;

		typedef class _lexer :
				protected lexer_base {
			
			public:

				_lexer(void);

				_lexer(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				_lexer(
					__in const _lexer &other
					);

				virtual ~_lexer(void);

				_lexer &operator=(
					__in const _lexer &other
					);

				virtual void clear(void);

				virtual size_t discover(void);

				token &get_begin_token(void);

				token &get_end_token(void);

				token &get_token(void);

				token &get_token(
					__in size_t position
					);

				size_t get_token_position(void);

				bool has_next_token(void);

				bool has_previous_token(void);

				token &move_next_token(
					__in bool expect_next = false
					);

				token &move_previous_token(
					__in bool expect_previous = false
					);

				virtual void reset(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual size_t size(void);

				virtual std::string to_string(
					__in bool verbose = false
					);

			protected:

				void add_token(
					__in const uuid &id
					);

				void add_token(
					__in const uuid &id,
					__in size_t position
					);

				uuid create_token(
					__in_opt token_t type = TOKEN_BEGIN,
					__in_opt uint32_t subtype = TOKEN_SUBTYPE_INVALID,
					__in_opt const std::string &text = std::string(),
					__in_opt const std::string &source = std::string(),
					__in_opt size_t line = 0,
					__in_opt double value = 0
					);

				void enumerate_alpha_token(void);

				void enumerate_digit_token(void);

				void enumerate_digit_token_subtype(
					__inout uint32_t &subtype
					);

				void enumerate_digit_token_value(
					__inout double &value,
					__in const std::string &text,
					__in token_t type,
					__in uint32_t subtype
					);

				void enumerate_escape_character(
					__inout char &ch
					);

				void enumerate_literal_character_token(
					__inout token_t &type,
					__inout double &value,
					__in size_t line
					);

				void enumerate_literal_string_token(
					__inout token_t &type,
					__inout std::string &text,
					__in size_t line
					);

				void enumerate_symbol_token(void);

				void enumerate_symbol_token_helper(
					__inout token_t &type,
					__inout uint32_t &subtype
					);

				void enumerate_token(void);

				token_factory_ptr get_token_factory(void);

				void skip_whitespace(void);

				std::vector<uuid> m_token_list;

				size_t m_token_position;

			private:

				std::recursive_mutex m_lock;

		} lexer, *lexer_ptr;
	}
}

#endif // LUNA_LEXER_H_
