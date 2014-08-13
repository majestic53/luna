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

#include "luna.h"
#include "luna_lexer_type.h"

namespace LUNA_NS {

	namespace LUNA_LANG_NS {
	
		#define CHARACTER_BINARY_MIN '0'
		#define CHARACTER_BINARY_MAX '1'
		#define CHARACTER_COMMENT '#'
		#define CHARACTER_DECIMAL '.'
		#define CHARACTER_DECIMAL_MIN '0'
		#define CHARACTER_DECIMAL_MAX '9'
		#define CHARACTER_DELIMITER_CHARACTER '\''
		#define CHARACTER_DELIMITER_STRING '\"'
		#define CHARACTER_ESCAPE '\\'
		#define CHARACTER_ESCAPE_ALARM 'a'
		#define CHARACTER_ESCAPE_BACKSLASH '\\'
		#define CHARACTER_ESCAPE_BACKSPACE 'b'
		#define CHARACTER_ESCAPE_CARRIAGE_RETURN 'r'
		#define CHARACTER_ESCAPE_DELIMITER_CHARACTER '\''
		#define CHARACTER_ESCAPE_DELIMITER_STRING '\"'
		#define CHARACTER_ESCAPE_FORMFEED 'f'
		#define CHARACTER_ESCAPE_HEXIDECIMAL 'x'
		#define CHARACTER_ESCAPE_HEXIDECIMAL_LENGTH 2
		#define CHARACTER_ESCAPE_NEWLINE 'n'
		#define CHARACTER_ESCAPE_OCTAL_LENGTH 3
		#define CHARACTER_ESCAPE_QUESTION_MARK '?'
		#define CHARACTER_ESCAPE_TAB_HORIZONTAL 't'
		#define CHARACTER_ESCAPE_TAB_VERTICAL 'v'
		#define CHARACTER_HEXIDECIMAL_MIN 'a'
		#define CHARACTER_HEXIDECIMAL_MAX 'f'
		#define CHARACTER_NEWLINE '\n'
		#define CHARACTER_NUMERIC_PREFIX_BINARY 'b'
		#define CHARACTER_NUMERIC_PREFIX_HEXIDECIMAL 'x'
		#define CHARACTER_NUMERIC_PREFIX_OCTAL '0'
		#define CHARACTER_OCTAL_MIN '0'
		#define CHARACTER_OCTAL_MAX '7'
		#define CHARACTER_UNDERSCORE '_'

		static const std::string CHARACTER_STR[] = {
			"ALPHA", "DIGIT", "SYMBOL", "WHITESPACE",
			};

		#define CHARACTER_STRING(_TYPE_)\
			((_TYPE_) > CHARACTER_MAX ? UNKNOWN : CHARACTER_STR[_TYPE_].c_str())

		enum {
			ESCAPE_ALARM = 0,
			ESCAPE_BACKSLASH,
			ESCAPE_BACKSPACE,
			ESCAPE_CARRIAGE_RETURN,
			ESCAPE_DELIMITER_CHARACTER,
			ESCAPE_DELIMITER_STRING,
			ESCAPE_FORMFEED,
			ESCAPE_HEXIDECIMAL,
			ESCAPE_NEWLINE,
			ESCAPE_QUESTION_MARK,
			ESCAPE_TAB_HORIZONTAL,
			ESCAPE_TAB_VERTICAL,
		};

		#define ESCAPE_MAX ESCAPE_TAB_VERTICAL

		static const char ESCAPE_CHARACTER_VAL[] = {
			'\a', CHARACTER_ESCAPE_BACKSLASH, '\b', '\r', CHARACTER_DELIMITER_CHARACTER, CHARACTER_DELIMITER_STRING, 
			'\f', CHARACTER_ESCAPE_HEXIDECIMAL, CHARACTER_NEWLINE, '\?', '\t', '\v',
			};
	
		#define ESCAPE_CHARACTER_VALUE(_TYPE_)\
			((_TYPE_) > ESCAPE_MAX ? CHARACTER_EOS : ESCAPE_CHARACTER_VAL[_TYPE_])

		enum {
			RADIX_BINARY = 2,
			RADIX_DECIMAL = 10,
			RADIX_HEXIDECIMAL = 16,
			RADIX_OCTAL = 8,
		};

		#define RADIX_MAX RADIX_OCTAL

		_lexer_base::_lexer_base(void)
		{
			TRACE_ENTRY();

			lexer_base::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer_base::_lexer_base(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();

			lexer_base::set(input, is_file);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer_base::_lexer_base(
			__in const _lexer_base &other
			) :
				luna_lang_base(other),
				m_char_column(other.m_char_column),
				m_char_column_map(other.m_char_column_map),
				m_char_position(other.m_char_position),
				m_char_row(other.m_char_row),
				m_char_type(other.m_char_type)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer_base::~_lexer_base(void)
		{
			TRACE_ENTRY();

			lexer_base::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer_base &
		_lexer_base::operator=(
			__in const _lexer_base &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				luna_lang_base::operator=(other);
				m_char_column = other.m_char_column;
				m_char_column_map = other.m_char_column_map;
				m_char_position = other.m_char_position;
				m_char_row = other.m_char_row;
				m_char_type = other.m_char_type;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_lexer_base::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			luna_lang_base::clear();
			m_char_column = 0;
			m_char_column_map.clear();
			m_char_position = 0;
			m_char_row = 0;
			determine_character_type();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer_base::determine_character_type(void)
		{
			m_char_type = determine_character_type(m_char_position);
		}

		char_t 
		_lexer_base::determine_character_type(
			__in size_t position
			)
		{
			char ch;
			char_t type;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			ch = get_character(position);
			if(isalpha(ch)) {
				type = CHARACTER_ALPHA;
			} else if(isdigit(ch)) {
				type = CHARACTER_DIGIT;
			} else if(isspace(ch)) {
				type = CHARACTER_WHITESPACE;
			} else {
				type = CHARACTER_SYMBOL;
			}

			TRACE_EXIT("Return Value: %s", CHARACTER_STRING(type));
			return type;
		}

		size_t 
		_lexer_base::discover(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_text.size() - 1);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		char 
		_lexer_base::get_character(void)
		{
			return get_character(m_char_position);
		}

		char 
		_lexer_base::get_character(
			__in size_t position
			)
		{
			char result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position >= m_text.size()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_CHARACTER_POSITION,
					"pos/max: %lu/%lu", position, (m_text.size() - 1));
			}

			result = m_text.at(position);

			TRACE_EXIT("Return Value: \'%c\' (%lu/%lu)", result, position, 
				(m_text.size() - 1));
			return result;	
		}

		size_t 
		_lexer_base::get_character_column(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_char_column);
			return m_char_column;
		}

		size_t 
		_lexer_base::get_character_position(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_char_position);
			return m_char_position;
		}

		size_t 
		_lexer_base::get_character_row(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_char_row);
			return m_char_row;
		}

		char_t 
		_lexer_base::get_character_type(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %s", CHARACTER_STRING(m_char_type));
			return m_char_type;
		}

		bool 
		_lexer_base::has_next_character(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_text.size() > 1) ? (m_char_position <= (m_text.size() - 2)) : false;

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_lexer_base::has_previous_character(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_char_position > 0);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		char 
		_lexer_base::move_next_character(
			__in_opt bool expect_next
			)
		{
			char ch;
			std::map<size_t, size_t>::iterator col_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
	
			if(!has_next_character()) {
				THROW_LUNA_LEXER_EXCEPTION(expect_next ? LUNA_LEXER_EXCEPTION_EXPECTING_NEXT_CHARACTER 
					: LUNA_LEXER_EXCEPTION_NO_NEXT_CHARACTER);
			}

			ch = get_character();
			if(ch == CHARACTER_NEWLINE) {
				
				col_iter = m_char_column_map.find(m_char_row);
				if(col_iter != m_char_column_map.end()) {
					THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_CHARACTER_ROW,
						"row/col: %lu/%lu", m_char_row, m_char_column);
				}

				m_char_column_map.insert(std::pair<size_t, size_t>(m_char_row, m_char_column));
				m_char_column = 0;
				++m_char_row;
			} else {
				++m_char_column;
			}

			++m_char_position;
			determine_character_type();

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_character();
		}

		char 
		_lexer_base::move_previous_character(
			__in_opt bool expect_previous
			)
		{
			char ch;
			std::map<size_t, size_t>::iterator col_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!has_previous_character()) {
				THROW_LUNA_LEXER_EXCEPTION(expect_previous ? LUNA_LEXER_EXCEPTION_EXPECTING_PREVIOUS_CHARACTER 
					: LUNA_LEXER_EXCEPTION_NO_PREVIOUS_CHARACTER);
			}

			--m_char_position;
			determine_character_type();

			ch = get_character();
			if(ch == CHARACTER_NEWLINE) {
				--m_char_row;

				col_iter = m_char_column_map.find(m_char_row);
				if(col_iter == m_char_column_map.end()) {
					THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_CHARACTER_ROW,
						"row/col: %lu/%lu", m_char_row, m_char_column);
				}

				m_char_column = col_iter->second;
			} else {
				--m_char_column;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_character();
		}

		void 
		_lexer_base::reset(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_char_column = 0;
			m_char_column_map.clear();
			m_char_position = 0;
			m_char_row = 0;
			determine_character_type();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer_base::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();

			luna_lang_base::set(input, is_file);
			lexer_base::reset();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_lexer_base::to_string(
			__in_opt bool verbose
			)
		{
			char ch;
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			ch = get_character();

			if(verbose) {
				result << "(0x" << VALUE_AS_HEX(uint8_t, ch) 
				<< ", row/col/pos: " << m_char_row << "/" << m_char_column << "/" 
				<< m_char_position << ") [" << CHARACTER_STRING(m_char_type) << "] ";
			}

			result << "\'" << (isprint(ch) ? ch : ' ') << "\'";

			if(!verbose) {
				result << " (ln. " << m_char_row << ")";
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_lexer::_lexer(void)
		{
			TRACE_ENTRY();

			lexer::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer::_lexer(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();

			lexer::set(input, is_file);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer::_lexer(
			__in const _lexer &other
			) :
				lexer_base(other),
				m_token_list(other.m_token_list),
				m_token_position(other.m_token_position)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer::~_lexer(void)
		{
			TRACE_ENTRY();

			lexer::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_lexer &
		_lexer::operator=(
			__in const _lexer &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				lexer_base::operator=(other);
				lexer::clear();
				m_token_list = other.m_token_list;
				m_token_position = other.m_token_position;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_lexer::add_token(
			__in const uuid &id
			)
		{
			add_token(id, m_token_position + 1);
		}

		void 
		_lexer::add_token(
			__in const uuid &id,
			__in size_t position
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			 
			if(position > m_token_list.size()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_TOKEN_POSITION,
					"pos/max: %lu/%lu", position, m_token_list.size());
			}

			if(position < m_token_list.size()) {
				m_token_list.insert(m_token_list.begin() + position, id);
			} else {
				m_token_list.push_back(id);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::clear(void)
		{
			token_factory_ptr tok_fact;
			std::vector<uuid>::iterator id_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			lexer_base::reset();
			skip_whitespace();

			tok_fact = get_token_factory();
			if(tok_fact) {

				for(id_iter = m_token_list.begin(); id_iter != m_token_list.end(); ++id_iter) {
					tok_fact->remove_token(*id_iter);
				}
			}

			m_token_list.clear();

			if(tok_fact) {
				m_token_list.push_back(create_token(TOKEN_BEGIN));
				m_token_list.push_back(create_token(TOKEN_END));
			}

			m_token_position = 0;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		uuid 
		_lexer::create_token(
			__in_opt token_t type,
			__in_opt uint32_t subtype,
			__in_opt const std::string &text,
			__in_opt const std::string &source,
			__in_opt size_t line,
			__in_opt double value
			)	
		{
			token tok(type, subtype);
			token_factory_ptr tok_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			tok_fact = get_token_factory();
			if(!tok_fact) {
				THROW_LUNA_LEXER_EXCEPTION(LUNA_LEXER_EXCEPTION_FACTORY_ALLOC_FAILED);
			}

			tok.set_text(text);
			tok.set_value(value);
			tok.set_source(source, line);
			tok_fact->add_token(tok.get_id(), tok);

			TRACE_EXIT("Return Value: %s", tok.to_string().c_str());
			return tok.get_id();
		}

		size_t 
		_lexer::discover(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			lexer::reset();

			while(lexer::has_next_token()) {
				lexer::move_next_token();
			}

			result = lexer::size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		void 
		_lexer::enumerate_alpha_token(void)
		{
			size_t line;
			std::string text;
			token_t type = TOKEN_IDENTIFIER;
			uint32_t subtype = TOKEN_SUBTYPE_INVALID;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer_base::get_character_type() != CHARACTER_ALPHA)
					&& (lexer_base::get_character() != CHARACTER_UNDERSCORE)) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_ALPHA_CHARACTER,
					"%s", lexer_base::to_string().c_str());
			}

			line = lexer_base::get_character_row();
			text += lexer_base::get_character();
			lexer_base::move_next_character();

			for(;;) {
				
				if((lexer_base::get_character_type() != CHARACTER_ALPHA)
						&& (lexer_base::get_character_type() != CHARACTER_DIGIT)
						&& (lexer_base::get_character() != CHARACTER_UNDERSCORE)) {
					break;
				}

				text += lexer_base::get_character();

				if(!lexer_base::has_next_character()) {
					break;
				}

				lexer_base::move_next_character();
			}

			if(IS_BOOLEAN(text)) {
				type = TOKEN_BOOLEAN;
				subtype = determine_token_subtype(text, type);
				text.clear();
			} else if(IS_CONTROL(text)) {
				type = TOKEN_CONTROL;
				subtype = determine_token_subtype(text, type);
				text.clear();
			} else if(IS_KEYWORD(text)) {
				type = TOKEN_KEYWORD;
				subtype = determine_token_subtype(text, type);
				text.clear();
			} else if(IS_NULL(text)) {
				type = TOKEN_NULL;
				subtype = determine_token_subtype(text, type);
				text.clear();
			}

			add_token(create_token(type, subtype, text, lexer_base::get_source(), line).get_id());

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_digit_token(void)
		{
			char ch;
			size_t line;
			std::string text;
			double value = 0;
			token_t type = TOKEN_SCALAR;
			uint32_t subtype = SCALAR_DECIMAL;
			bool enumerated = false, tail_enumerated = false;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer_base::get_character_type() != CHARACTER_DIGIT) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_DIGIT_CHARACTER,
					"%s", lexer_base::to_string().c_str());
			}

			line = lexer_base::get_character_row();
			ch = lexer_base::get_character();
			enumerate_digit_token_subtype(subtype);

			if((subtype != SCALAR_BINARY)
					&& (subtype != SCALAR_HEXIDECIMAL)) {
				text += ch;
			}

			for(;;) {
				
				if(!lexer_base::has_next_character()) {
					break;
				}

				ch = TO_LOWER_CHAR(lexer_base::move_next_character());
				

				switch(type) {
				case TOKEN_FLOAT:

					if((ch < CHARACTER_DECIMAL_MIN) || (ch > CHARACTER_DECIMAL_MAX)) {

						if(!tail_enumerated) {
							THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_SCALAR_FLOAT_VALUE,
								"%s", lexer_base::to_string().c_str());
						}

						enumerated = true;
					} else if(!tail_enumerated) {
						tail_enumerated = true;
					}
					break;
				default:

					switch(subtype) {
						case SCALAR_BINARY:

							if((ch < CHARACTER_BINARY_MIN) || (ch > CHARACTER_BINARY_MAX)) {
								enumerated = true;
							}
							break;
						case SCALAR_DECIMAL:

							if(ch == CHARACTER_DECIMAL) {
								type = TOKEN_FLOAT;
								subtype = TOKEN_SUBTYPE_INVALID;
							} else if((ch < CHARACTER_DECIMAL_MIN) || (ch > CHARACTER_DECIMAL_MAX)) {
								enumerated = true;
							}
							break;
						case SCALAR_HEXIDECIMAL:

							if(((ch < CHARACTER_DECIMAL_MIN) || (ch > CHARACTER_DECIMAL_MAX))
									&& ((ch < CHARACTER_HEXIDECIMAL_MIN) || (ch > CHARACTER_HEXIDECIMAL_MAX))) {
								enumerated = true;
							}
							break;
						case SCALAR_OCTAL:

							if((ch < CHARACTER_OCTAL_MIN) || (ch > CHARACTER_OCTAL_MAX)) {
								enumerated = true;
							}
							break;
						default:
							THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNKNOWN_SCALAR_TYPE,
								"%s", lexer_base::to_string().c_str());
					}
					break;
				}

				if(enumerated) {
					break;
				}

				text += ch;
			}

			if(text.empty()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_SCALAR_VALUE,
					"%s", lexer_base::to_string().c_str());
			}

			enumerate_digit_token_value(value, text, type, subtype);
			text.clear();
			add_token(create_token(type, subtype, text, lexer_base::get_source(), line, value).get_id());

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_digit_token_subtype(
			__inout uint32_t &subtype
			)
		{
			char ch;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			ch = lexer_base::get_character();
			if(ch == CHARACTER_NUMERIC_PREFIX_OCTAL) {
				subtype = SCALAR_OCTAL;

				if(lexer_base::has_next_character()) {
					ch = lexer_base::move_next_character();

					switch(ch) {
						case CHARACTER_NUMERIC_PREFIX_BINARY:
							subtype = SCALAR_BINARY;
							break;
						case CHARACTER_NUMERIC_PREFIX_HEXIDECIMAL:
							subtype = SCALAR_HEXIDECIMAL;
							break;
						default:

							if(!lexer_base::has_previous_character()) {
								THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_SCALAR_PARSE_FAILED,
									"%s", lexer_base::to_string().c_str());
							}

							lexer_base::move_previous_character();
							break;
					}
				}
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_digit_token_value(
			__inout double &value,
			__in const std::string &text,
			__in token_t type,
			__in uint32_t subtype
			)
		{
			char *end = NULL;
			int radix = RADIX_DECIMAL;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			switch(type) {
			case TOKEN_FLOAT:
				value = atof(text.c_str());
				break;
			default:

				switch(subtype) {
					case SCALAR_BINARY:
						radix = RADIX_BINARY;
						break;
					case SCALAR_DECIMAL:
						radix = RADIX_DECIMAL;
						break;
					case SCALAR_HEXIDECIMAL:
						radix = RADIX_HEXIDECIMAL;
						break;
					case SCALAR_OCTAL:
						radix = RADIX_OCTAL;
						break;
					default:
						THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNKNOWN_SCALAR_TYPE,
							"%s (%lu)", lexer_base::to_string().c_str(), subtype);
				}

				value = (double) strtol(text.c_str(), &end, radix);
				break;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_escape_character(
			__inout char &ch
			)
		{
			double value;
			std::string text;
			size_t ch_iter = 0;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!lexer_base::has_next_character()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_ESCAPE_SEQUENCE,
					"%s", lexer_base::to_string().c_str());
			}

			ch = lexer_base::move_next_character();
			switch(ch) {
				case CHARACTER_ESCAPE_ALARM:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_ALARM);
					break;
				case CHARACTER_ESCAPE_BACKSLASH:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_BACKSLASH);
					break;
				case CHARACTER_ESCAPE_BACKSPACE:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_BACKSPACE);
					break;
				case CHARACTER_ESCAPE_CARRIAGE_RETURN:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_CARRIAGE_RETURN);
					break;
				case CHARACTER_ESCAPE_DELIMITER_CHARACTER:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_DELIMITER_CHARACTER);
					break;
				case CHARACTER_ESCAPE_DELIMITER_STRING:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_DELIMITER_STRING);
					break;
				case CHARACTER_ESCAPE_FORMFEED:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_FORMFEED);
					break;
				case CHARACTER_ESCAPE_HEXIDECIMAL:

					for(; ch_iter < CHARACTER_ESCAPE_HEXIDECIMAL_LENGTH; ++ch_iter) {

						if(!lexer_base::has_next_character()) {
							THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_ESCAPE_SEQUENCE,
								"%s", lexer_base::to_string().c_str());
						}

						ch = lexer_base::move_next_character();
						if(((ch < CHARACTER_DECIMAL_MIN) || (ch > CHARACTER_DECIMAL_MAX))
								&& ((ch < CHARACTER_HEXIDECIMAL_MIN) || (ch > CHARACTER_HEXIDECIMAL_MAX))) {
							THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_ESCAPE_SEQUENCE,
								"%s", lexer_base::to_string().c_str());
						}

						text += ch;
					}

					enumerate_digit_token_value(value, text, TOKEN_SCALAR, SCALAR_HEXIDECIMAL);
					ch = (char) value;
					break;
				case CHARACTER_ESCAPE_NEWLINE:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_NEWLINE);
					break;
				case CHARACTER_ESCAPE_QUESTION_MARK:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_QUESTION_MARK);
					break;
				case CHARACTER_ESCAPE_TAB_HORIZONTAL:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_TAB_HORIZONTAL);
					break;
				case CHARACTER_ESCAPE_TAB_VERTICAL:
					ch = ESCAPE_CHARACTER_VALUE(ESCAPE_TAB_VERTICAL);
					break;
				default:

					for(; ch_iter < CHARACTER_ESCAPE_OCTAL_LENGTH; ++ch_iter) {
						ch = lexer_base::get_character();
						
						if((ch < CHARACTER_OCTAL_MIN) || (ch > CHARACTER_OCTAL_MAX)) {
							THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_ESCAPE_SEQUENCE,
								"%s", lexer_base::to_string().c_str());
						}

						text += ch;

						if(!lexer_base::has_next_character()) {
							THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_ESCAPE_SEQUENCE,
								"%s", lexer_base::to_string().c_str());
						}

						if(ch_iter < (CHARACTER_ESCAPE_OCTAL_LENGTH - 1)) {
							lexer_base::move_next_character();
						}
					}

					enumerate_digit_token_value(value, text, TOKEN_SCALAR, SCALAR_OCTAL);
					ch = (char) value;
					break;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_literal_character_token(
			__inout token_t &type,
			__inout double &value,
			__in size_t line
			)
		{
			char ch;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			ch = lexer_base::get_character();
			if(ch != CHARACTER_DELIMITER_CHARACTER) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_CHARACTER_DELIMITER,
					"%s", lexer_base::to_string().c_str());
			}

			type = TOKEN_LITERAL_CHARACTER;

			if(!lexer_base::has_next_character()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_CHARACTER_LITERAL,
					"ln. %lu", line);
			}

			ch = lexer_base::move_next_character();
			if(ch == CHARACTER_DELIMITER_CHARACTER) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_CHARACTER_LITERAL,
					"%s", lexer_base::to_string().c_str());
			} else if(ch == CHARACTER_ESCAPE) {
				enumerate_escape_character(ch);
			}

			value = ch;

			ch = lexer_base::move_next_character();
			if(ch != CHARACTER_DELIMITER_CHARACTER) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_CHARACTER_LITERAL,
					"%s", lexer_base::to_string().c_str());
			}

			if(lexer_base::has_next_character()) {
				lexer_base::move_next_character();
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_literal_string_token(
			__inout token_t &type,
			__inout std::string &text,
			__in size_t line
			)
		{
			char ch;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			ch = lexer_base::get_character();
			if(ch != CHARACTER_DELIMITER_STRING) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_STRING_DELIMITER,
					"%s", lexer_base::to_string().c_str());
			}

			type = TOKEN_LITERAL_STRING;

			if(!lexer_base::has_next_character()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL,
					"line %lu", line);
			}

			ch = lexer_base::move_next_character();

			for(;;) {
						
				if(!lexer_base::has_next_character()) {
					THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL,
						"line %lu", line);
				}

				if(ch == CHARACTER_DELIMITER_STRING) {
					break;
				}

				if(ch == CHARACTER_ESCAPE) {
					enumerate_escape_character(ch);
				}

				text += ch;
				ch = lexer_base::move_next_character();
			}

			if(lexer_base::has_next_character()) {
				lexer_base::move_next_character();
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_symbol_token(void)
		{
			char ch;
			size_t line;
			std::string text;
			double value = 0;
			bool add_tok = true;
			token_t type = TOKEN_SYMBOL;
			uint32_t subtype = TOKEN_SUBTYPE_INVALID;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer_base::get_character_type() != CHARACTER_SYMBOL) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_SYMBOL_CHARACTER,
					"%s", lexer_base::to_string().c_str());
			}

			line = lexer_base::get_character_row();

			ch = lexer_base::get_character();
			switch(ch) {
				case CHARACTER_DELIMITER_CHARACTER:
					enumerate_literal_character_token(type, value, line);
					break;
				case CHARACTER_DELIMITER_STRING:
					enumerate_literal_string_token(type, text, line);
					break;
				case CHARACTER_UNDERSCORE:
					enumerate_alpha_token();
					add_tok = false;
					break;
				default:
					enumerate_symbol_token_helper(type, subtype);
					break;
			}

			if(add_tok) {
				add_token(create_token(type, subtype, text, lexer_base::get_source(), line, value).get_id());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_symbol_token_helper(
			__inout token_t &type,
			__inout uint32_t &subtype
			)
		{
			char ch;
			std::string ext_text, text;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer_base::get_character_type() != CHARACTER_SYMBOL) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_EXPECTING_SYMBOL_CHARACTER,
					"%s", lexer_base::to_string().c_str());
			}

			ch = lexer_base::get_character();
			text += ch;

			if(IS_ARITHMETIC_OPERATOR(text)) {
				type = TOKEN_ARITHMETIC_OPERATOR;
			} else if(IS_BINARY_OPERATOR(text)) {
				type = TOKEN_BINARY_OPERATOR;
			} else if(IS_OPERATOR(text)) {
				type = TOKEN_OPERATOR;
			} else if(IS_SYMBOL(text)) {
				type = TOKEN_SYMBOL;
			} else {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_UNKNOWN_SYMBOL_CHARACTER,
					"%s", lexer_base::to_string().c_str());
			}

			if(lexer_base::has_next_character()) {
				ch = lexer_base::move_next_character();

				if(lexer_base::get_character_type() == CHARACTER_SYMBOL) {
					ext_text = text;
					ext_text += ch;
				}

				if(IS_OPERATOR(ext_text)) {
					type = TOKEN_OPERATOR;
					text = ext_text;

					if(lexer_base::has_next_character()) {
						lexer_base::move_next_character();
					}
				} else if(IS_SHIFT_OPERATOR(ext_text)) {
					type = TOKEN_SHIFT_OPEATOR;
					text = ext_text;

					if(lexer_base::has_next_character()) {
						lexer_base::move_next_character();
					}
				}
			}

			subtype = determine_token_subtype(text, type);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::enumerate_token(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			switch(lexer_base::get_character_type()) {
				case CHARACTER_ALPHA:
					enumerate_alpha_token();
					break;
				case CHARACTER_DIGIT:
					enumerate_digit_token();
					break;
				default:
					enumerate_symbol_token();
					break;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		token &
		_lexer::get_begin_token(void)
		{
			return get_token(0);
		}

		token &
		_lexer::get_end_token(void)
		{
			return get_token(m_token_list.size() - 1);
		}

		token &
		_lexer::get_token(void)
		{
			return get_token(m_token_position);
		}

		token &
		_lexer::get_token(
			__in size_t position
			)
		{
			token_factory_ptr tok_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position >= m_token_list.size()) {
				THROW_LUNA_LEXER_EXCEPTION_MESSAGE(LUNA_LEXER_EXCEPTION_INVALID_TOKEN_POSITION,
					"pos/max: %lu/%lu", position, (m_token_list.size() - 1));
			}

			tok_fact = get_token_factory();
			if(!tok_fact) {
				THROW_LUNA_LEXER_EXCEPTION(LUNA_LEXER_EXCEPTION_FACTORY_ALLOC_FAILED);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return tok_fact->get_token(m_token_list.at(position));
		}

		token_factory_ptr 
		_lexer::get_token_factory(void)
		{
			token_factory_ptr result = NULL;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(luna::is_globally_initialized()) {

				if(get_context()->is_initialized()) {
					result = get_context()->acquire_token_factory();
				}
			}

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		size_t 
		_lexer::get_token_position(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_token_position);
			return m_token_position;
		}

		bool 
		_lexer::has_next_token(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (get_token().get_type() != TOKEN_END);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_lexer::has_previous_token(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_token_position > 0);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		token &
		_lexer::move_next_token(
			__in bool expect_next
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!has_next_token()) {
				THROW_LUNA_LEXER_EXCEPTION(expect_next ? LUNA_LEXER_EXCEPTION_EXPECTING_NEXT_TOKEN 
					: LUNA_LEXER_EXCEPTION_NO_NEXT_TOKEN);
			}

			if(lexer_base::has_next_character() 
					&& (m_token_position == (m_token_list.size() - SENTINEL_COUNT))) {
				enumerate_token();
				skip_whitespace();
			}

			++m_token_position;

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_token();
		}

		token &
		_lexer::move_previous_token(
			__in bool expect_previous
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!has_previous_token()) {
				THROW_LUNA_LEXER_EXCEPTION(expect_previous ? LUNA_LEXER_EXCEPTION_EXPECTING_PREVIOUS_TOKEN 
					: LUNA_LEXER_EXCEPTION_NO_PREVIOUS_TOKEN);
			}

			--m_token_position;

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_token();
		}

		void 
		_lexer::reset(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_token_position = 0;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_lexer::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			lexer_base::set(input, is_file);
			lexer::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_lexer::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_token_list.size() - SENTINEL_COUNT);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		void 
		_lexer::skip_whitespace(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			for(;;) {
				
				if(!lexer_base::has_next_character()
						|| (lexer_base::get_character_type() != CHARACTER_WHITESPACE)) {
					break;
				}

				lexer_base::move_next_character();
			}

			if(lexer_base::get_character() == CHARACTER_COMMENT) {
				
				for(;;) {
				
					if(!lexer_base::has_next_character()) {
						break;
					}

					if(lexer_base::get_character() == CHARACTER_NEWLINE) {
						lexer_base::move_next_character();
						break;
					}

					lexer_base::move_next_character();
				}

				if(lexer_base::has_next_character()) {
					skip_whitespace();
				}
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_lexer::to_string(
			__in bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << "(" << m_token_position << "/" << (m_token_list.size() - 1) << ") ";
			}

			result << get_token().to_string(verbose);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}
