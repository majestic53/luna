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

#include <fstream>
#include "luna.h"
#include "luna_language_type.h"

namespace LUNA_NS {

	namespace LUNA_LANG_NS {

		uint32_t 
		determine_token_subtype(
			__in const std::string &text,
			__in token_t type
			)
		{
			size_t subtype_str_len;
			uint32_t result = TOKEN_SUBTYPE_INVALID;

			TRACE_ENTRY();

			subtype_str_len = TOKEN_SUBTYPE_STRINGS_LENGTH(type);
			if(subtype_str_len) {
				
				const std::string *subtype_str = TOKEN_SUBTYPE_STRINGS(type);
				if(subtype_str) {

					for(result = 0; result < subtype_str_len; ++result) {
						
						if(text == subtype_str[result]) {
							break;
						}
					}
				}

				if(result == subtype_str_len) {
					result = TOKEN_SUBTYPE_INVALID;
				}
			}

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		_luna_lang_base::_luna_lang_base(void) :
			m_context(NULL)
		{
			TRACE_ENTRY();

			luna_lang_base::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_luna_lang_base::_luna_lang_base(
			__in const std::string &input,
			__in_opt bool is_file
			) :
				m_context(NULL)
		{
			TRACE_ENTRY();

			luna_lang_base::set(input, is_file);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_luna_lang_base::_luna_lang_base(
			__in const _luna_lang_base &other
			) :
				m_context(other.m_context),
				m_source(other.m_source),
				m_text(other.m_text)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_luna_lang_base::~_luna_lang_base(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_luna_lang_base &
		_luna_lang_base::operator=(
			__in const _luna_lang_base &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				m_context = other.m_context;
				m_source = other.m_source;
				m_text = other.m_text;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_luna_lang_base::acquire_context(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_context) {
				
				m_context = luna::acquire();
				if(!m_context) {
					THROW_LUNA_LANGUAGE_EXCEPTION(LUNA_LANGUAGE_EXCEPTION_CONTEXT_ALLOC_FAILED);
				}
			}

			TRACE_EXIT("Return Value: 0x%p", m_context);
		}

		void 
		_luna_lang_base::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			acquire_context();
			m_source.clear();
			m_text.clear();
			m_text += CHARACTER_EOS;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_luna *
		_luna_lang_base::get_context(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%p", m_context);
			return m_context;
		}

		std::string &
		_luna_lang_base::get_source(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_source;
		}

		std::string &
		_luna_lang_base::get_text(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_text;
		}

		bool 
		_luna_lang_base::is_file(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = !m_source.empty();

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		size_t 
		_luna_lang_base::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_text.size() - 1);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_luna_lang_base::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			std::stringstream stream;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			luna_lang_base::clear();

			if(is_file) {
				std::ifstream file(input.c_str(), std::ios::in);

				if(!file) {
					THROW_LUNA_LANGUAGE_EXCEPTION_MESSAGE(LUNA_LANGUAGE_EXCEPTION_FILE_NOT_FOUND,
						"\'%s\'", input.c_str());
				}

				m_source = input;
				stream << file.rdbuf();
				m_text = stream.str();
				file.close();
			} else {
				m_source.clear();
				m_text = input;
			}

			m_text += CHARACTER_EOS;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_luna_lang_base::to_string(
			__in_opt bool verbose
			)
		{
			size_t char_iter = 0;
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << "(";

			if(!m_source.empty()) {
				result << "file, \'" << m_source << "\'";
			} else {
				result << "non-file";
			}

			result << "(" << m_text.size() << ")) Context: 0x" << VALUE_AS_HEX(uintptr_t, m_context);

			if(verbose) {
				
				for(; char_iter < m_text.size(); ++char_iter) {
					
					if(!(char_iter % BLOCK_SIZE)) {
						result << std::endl << "0x" << VALUE_AS_HEX(size_t, char_iter) << " |";
					}

					result << " \'" << (isprint(m_text.at(char_iter)) ? m_text.at(char_iter) : ' ') 
						<< "\'(" << VALUE_AS_HEX(uint8_t, m_text.at(char_iter)) << ")";
				}
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}