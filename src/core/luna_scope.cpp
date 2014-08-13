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
#include "luna_scope_type.h"

namespace LUNA_NS {

	namespace LUNA_COMP_NS {
		
		_scope_factory *_scope_factory::m_instance = NULL;

		void 
		scope_factory_destroy(void)
		{
			TRACE_ENTRY();

			if(scope_factory::m_instance) {
				delete scope_factory::m_instance;
				scope_factory::m_instance = NULL;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope::_scope(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope::_scope(
			__in const _scope &other
			) :
				uuid_base(other),
				m_sym_table(other.m_sym_table)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope::~_scope(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope &
		_scope::operator=(
			__in const _scope &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				uuid_base::operator=(other);
				m_sym_table = other.m_sym_table;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_scope::add_symbol(
			__in const std::string &key,
			__in const uuid &node_id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_sym_table.find(key) != m_sym_table.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_SYMBOL_ALREADY_EXISTS,
					"%s", key.c_str());
			}

			if(key.empty()) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_KEY);
			}

			m_sym_table.insert(std::pair<std::string, uuid>(key, node_id));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_scope::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_sym_table.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::map<std::string, uuid>::iterator 
		_scope::find_symbol(
			__in const std::string &key
			)
		{
			std::map<std::string, uuid>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_sym_table.find(key);
			if(result == m_sym_table.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_SYMBOL_NOT_FOUND, 
					"%s", key.c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::get_symbol(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_symbol(key)->second;
		}

		bool 
		_scope::has_symbol(
			__in const std::string &key
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(key.empty()) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_KEY);
			}

			result = (m_sym_table.find(key) != m_sym_table.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_scope::remove_symbol(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_sym_table.erase(find_symbol(key));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_scope::set_symbol(
			__in const std::string &key,
			__in const uuid &node_id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			find_symbol(key)->second = node_id;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_scope::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_sym_table.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_scope::to_string(
			__in_opt bool verbose
			)
		{
			size_t count = 0;
			std::stringstream result;
			std::map<std::string, uuid>::iterator sym_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << uuid_base::to_string(verbose) << " ";
			}

			result << "Entries: " << m_sym_table.size();

			if(verbose) {
				
				for(sym_iter = m_sym_table.begin(); sym_iter != m_sym_table.end(); ++sym_iter,
						++count) {
					result << "--- [" << count << "/" << m_sym_table.size() << "] \'" << 
						sym_iter->first << "\'" << sym_iter->second.to_string();
				}
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_scope_factory::_scope_factory(
			__in_opt generic_cb_t dest
			)
		{
			TRACE_ENTRY();

			if(dest) {
				std::atexit(dest);
			} else {
				std::atexit(scope_factory_destroy);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope_factory::~_scope_factory(void)
		{
			TRACE_ENTRY();

			destroy();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope_factory *
		_scope_factory::acquire(
			__in_opt generic_cb_t dest
			)
		{
			scope_factory_ptr result = NULL;

			TRACE_ENTRY();

			if(!m_instance) {
			
				m_instance = new scope_factory(dest);
				if(!m_instance) {
					THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_ALLOC_FAILED);
				}
			}

			result = m_instance;

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_scope_factory::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			m_scope_map.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_scope_factory::destroy(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_initialized) {
				clear();
				m_initialized = false;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::map<uuid, scope>::iterator 
		_scope_factory::find_scope(
			__in const uuid &id
			)
		{
			std::map<uuid, scope>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			result = m_scope_map.find(id);
			if(result == m_scope_map.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_SCOPE_NOT_FOUND,
					"%s", uuid::id_as_string(id).c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		scope &
		_scope_factory::generate(void)
		{
			scope scop;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			m_scope_map.insert(std::pair<uuid, scope>(scop.get_id(), scop));

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_scope(scop.get_id())->second;
		}

		scope &
		_scope_factory::get_scope(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_scope(id)->second;
		}

		bool 
		_scope_factory::has_scope(
			__in const uuid &id
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			result = (m_scope_map.find(id) != m_scope_map.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_scope_factory::initialize(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			destroy();
			m_initialized = true;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		bool 
		_scope_factory::is_initialized(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", m_initialized);
			return m_initialized;
		}

		void 
		_scope_factory::remove_scope(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			m_scope_map.erase(find_scope(id));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_scope_factory::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			result = m_scope_map.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_scope_factory::to_string(
			__in bool verbose
			)
		{
			std::stringstream result;
			std::map<uuid, scope>::iterator scope_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_UNINITIALIZED);
			}

			if(verbose) {
				result << "(" << (m_initialized ? "initialized" : "uninitialized") << ") ";
			}

			result << "Entries: " << m_scope_map.size();

			for(scope_iter = m_scope_map.begin(); scope_iter != m_scope_map.end(); ++scope_iter) {
				result << std::endl << "---" << std::endl << scope_iter->second.to_string(verbose);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}
