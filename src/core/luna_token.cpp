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
#include "luna_token_type.h"

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		_token_factory *_token_factory::m_instance = NULL;

		void 
		token_factory_destroy(void)
		{
			TRACE_ENTRY();

			if(token_factory::m_instance) {
				delete token_factory::m_instance;
				token_factory::m_instance = NULL;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_token::_token(
			__in token_t type,
			__in_opt uint32_t subtype
			) :
				m_line(0),
				m_subtype(subtype),
				m_type(type),
				m_value(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_token::_token(
			__in const _token &other
			) :
				uuid_base(other),
				m_line(other.m_line),
				m_source(other.m_source),
				m_subtype(other.m_subtype),
				m_text(other.m_text),
				m_type(other.m_type),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_token::~_token(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_token &
		_token::operator=(
			__in const _token &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				uuid_base::operator=(other);
				m_line = other.m_line;
				m_source = other.m_source;
				m_subtype = other.m_subtype;
				m_text = other.m_text;
				m_type = other.m_type;
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_token::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_line = 0;
			m_source.clear();
			m_subtype = TOKEN_SUBTYPE_INVALID;
			m_text.clear();
			m_type = TOKEN_BEGIN;
			m_value = 0;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_token::find_subtype_string(
			__in token_t &type,
			__in uint32_t subtype
			)
		{
			std::string result = UNKNOWN;
			const std::string *subtype_iter = NULL;

			TRACE_ENTRY();

			if(type > TOKEN_MAX) {
				goto exit;
			}

			if(subtype >= TOKEN_SUBTYPE_STRINGS_LENGTH(type)) {
				goto exit;
			}

			subtype_iter = TOKEN_SUBTYPE_STRINGS(type);
			if(!subtype_iter) {
				goto exit;
			}

			result = subtype_iter[subtype];

exit:
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		size_t 
		_token::get_line(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_line);
			return m_line;
		}

		std::string &
		_token::get_source(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_source;
		}

		uint32_t 
		_token::get_subtype(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", m_subtype);
			return m_subtype;
		}

		std::string &
		_token::get_text(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_text;
		}

		token_t 
		_token::get_type(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", m_type);
			return m_type;
		}

		double 
		_token::get_value(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %g", m_value);
			return m_value;
		}

		bool 
		_token::has_source(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = !m_source.empty();

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_token::has_text(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = !m_text.empty();

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_token::set_source(
			__in const std::string &source,
			__in_opt size_t line
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_source = source;
			m_line = line;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_token::set_text(
			__in const std::string &text
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_text = text;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_token::set_type(
			__in token_t type,
			__in_opt uint32_t subtype
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_type = type;
			m_subtype = subtype;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_token::set_value(
			__in double value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_token::to_string(
			__in bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << uuid_base::to_string(verbose) << " ";
			}

			result << "[" << TOKEN_STRING(m_type);

			if(m_subtype != TOKEN_SUBTYPE_INVALID) {
				result << ", " << find_subtype_string(m_type, m_subtype);
			}

			result << "]";

			if(!m_text.empty()) {
				result << " \"" << m_text << "\"";
			} else if(m_type == TOKEN_LITERAL_STRING) {
				result << " \"\"";
			}

			switch(m_type) {
				case TOKEN_FLOAT:
				case TOKEN_SCALAR:
					result << " (" << m_value << ")";
					break;
				case TOKEN_LITERAL_CHARACTER:
					result << " \'" << (isprint((uint8_t) m_value) ? ((char) m_value) : ' ') << "\' " 
						<< "(0x" << VALUE_AS_HEX(uint8_t, m_value) << ")";
					break;
				case TOKEN_LITERAL_STRING:
					result << " (" << m_text.size() << ")";
					break;
			}

			if(!m_source.empty()) {
				result << " (" << m_source << "(" << m_line << "))";
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_token_factory::_token_factory(
			__in_opt generic_cb_t dest
			)
		{
			TRACE_ENTRY();

			if(dest) {
				std::atexit(dest);
			} else {
				std::atexit(token_factory_destroy);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_token_factory::~_token_factory(void)
		{
			TRACE_ENTRY();

			destroy();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_token_factory *
		_token_factory::acquire(
			__in_opt generic_cb_t dest
			)
		{
			token_factory_ptr result;

			TRACE_ENTRY();

			if(!m_instance) {
				
				m_instance = new token_factory(dest);
				if(!m_instance) {
					THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_ALLOC_FAILED);
				}
			}

			result = m_instance;

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_token_factory::add_token(
			__in const uuid &id,
			__in const token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			if(m_token_map.find(id) != m_token_map.end()) {
				THROW_LUNA_TOKEN_EXCEPTION_MESSAGE(LUNA_TOKEN_EXCEPTION_ID_ALREADY_ADDED,
					"%s", uuid::id_as_string(id).c_str());
			}

			m_token_map.insert(std::pair<uuid, token>(id, tok));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_token_factory::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			m_token_map.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_token_factory::destroy(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_initialized) {
				clear();
				m_initialized = false;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::map<uuid, token>::iterator 
		_token_factory::find_token(
			__in const uuid &id
			)
		{
			std::map<uuid, token>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			result = m_token_map.find(id);
			if(result == m_token_map.end()) {
				THROW_LUNA_TOKEN_EXCEPTION_MESSAGE(LUNA_TOKEN_EXCEPTION_ID_NOT_FOUND,
					"%s", uuid::id_as_string(id).c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		token &
		_token_factory::generate(
			__in_opt token_t type,
			__in_opt uint32_t subtype
			)
		{
			token tok;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			tok.set_type(type, subtype);
			add_token(tok.get_id(), tok);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_token(tok.get_id())->second;
		}

		token &
		_token_factory::get_token(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_token(id)->second;
		}

		bool 
		_token_factory::has_token(
			__in const uuid &id
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			result = (m_token_map.find(id) != m_token_map.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_token_factory::initialize(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			destroy();
			m_initialized = true;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		bool 
		_token_factory::is_initialized(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_initialized;

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_token_factory::remove_token(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			m_token_map.erase(find_token(id));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_token_factory::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			result = m_token_map.size();

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		std::string 
		_token_factory::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<uuid, token>::iterator tok_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_TOKEN_EXCEPTION(LUNA_TOKEN_EXCEPTION_UNINITIALIZED);
			}

			if(verbose) {
				result << "(" << (m_initialized ? "initialized" : "uninitialized") << ") ";
			}

			result << "Entries: " << m_token_map.size();
			
			for(tok_iter = m_token_map.begin(); tok_iter != m_token_map.end(); ++tok_iter) {
				result << std::endl << tok_iter->second.to_string(verbose);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}
