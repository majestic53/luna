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

#include <random>
#include "luna.h"
#include "luna_type.h"

namespace LUNA_NS {
	
	_luna *_luna::m_instance = NULL;
	bool _luna::m_glob_initalized = false;

	void 
	luna_destroy(void)
	{
		TRACE_ENTRY();

		if(luna::m_instance) {
			delete luna::m_instance;
			luna::m_instance = NULL;
		}

		luna::m_glob_initalized = false;

		TRACE_EXIT("Return Value: 0x%x", NULL);
	}

	_luna::_luna(
		__in_opt generic_cb_t dest
		) :
			m_initialized(false),
			m_node_factory(node_factory::acquire()),
			m_scope_factory(scope_factory::acquire()),
			m_token_factory(token_factory::acquire()),
			m_uuid_factory(uuid_factory::acquire())
	{
		TRACE_ENTRY();

		std::srand((uint32_t) std::time(NULL));

		if(dest) {
			std::atexit(dest);
		} else {
			std::atexit(luna_destroy);
		}

		TRACE_EXIT("Return Value: 0x%x", NULL);
	}

	_luna::~_luna(void)
	{
		TRACE_ENTRY();

		destroy();

		TRACE_EXIT("Return Value: 0x%x", NULL);
	}

	_luna * 
	_luna::acquire(
		__in_opt generic_cb_t dest
		)
	{
		luna_ptr result = NULL;

		TRACE_ENTRY();

		if(!m_instance) {
			
			m_instance = new luna(dest);
			if(!m_instance) {
				THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_ALLOC_FAILED);
			}

			m_glob_initalized = true;
		}

		result = m_instance;

		TRACE_EXIT("Return Value: 0x%p", result);
		return result;
	}

	node_factory_ptr 
	_luna::acquire_node_factory(void)
	{
		node_factory_ptr result = NULL;

		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(!m_node_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_UNINITIALIZED);
		}

		result = m_node_factory;

		TRACE_EXIT("Return Value: 0x%p", result);
		return result;
	}

	scope_factory_ptr 
	_luna::acquire_scope_factory(void)
	{
		scope_factory_ptr result = NULL;

		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(!m_scope_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_UNINITIALIZED);
		}

		result = m_scope_factory;

		TRACE_EXIT("Return Value: 0x%p", result);
		return result;
	}

	token_factory_ptr 
	_luna::acquire_token_factory(void)
	{
		token_factory_ptr result = NULL;

		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(!m_token_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_UNINITIALIZED);
		}

		result = m_token_factory;

		TRACE_EXIT("Return Value: 0x%p", result);
		return result;
	}

	uuid_factory_ptr 
	_luna::acquire_uuid_factory(void)
	{
		uuid_factory_ptr result = NULL;

		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_UNINITIALIZED);
		}

		if(!m_uuid_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_UNINITIALIZED);
		}

		result = m_uuid_factory;

		TRACE_EXIT("Return Value: 0x%p", result);
		return result;
	}

	void 
	_luna::destroy(void)
	{
		TRACE_ENTRY();
		SERIALIZE_CALL_RECUR(m_lock);

		if(m_initialized) {

			if(m_scope_factory) {
				m_scope_factory->destroy();
			}

			if(m_node_factory) {
				m_node_factory->destroy();
			}

			if(m_token_factory) {
				m_token_factory->destroy();
			}

			if(m_uuid_factory) {
				m_uuid_factory->destroy();
			}

			m_initialized = false;
		}

		TRACE_EXIT("Return Value: 0x%x", NULL);
	}

	void 
	_luna::initialize(void)
	{
		TRACE_ENTRY();
		SERIALIZE_CALL_RECUR(m_lock);

		destroy();

		if(!m_uuid_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_ALLOC_FAILED);
		}

		m_uuid_factory->initialize((std::rand() << 0x10) | std::rand());

		if(!m_token_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_ALLOC_FAILED);
		}

		m_token_factory->initialize();

		if(!m_node_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_ALLOC_FAILED);
		}

		m_node_factory->initialize();

		if(!m_scope_factory) {
			THROW_LUNA_EXCEPTION(LUNA_EXCEPTION_FACTORY_ALLOC_FAILED);
		}

		m_scope_factory->initialize();
		m_initialized = true;

		TRACE_EXIT("Return Value: 0x%x", NULL);
	}

	bool 
	_luna::is_globally_initialized(void)
	{
		bool result;

		TRACE_ENTRY();

		result = m_glob_initalized;

		TRACE_EXIT("Return Value: 0x%x", result);
		return result;
	}

	bool 
	_luna::is_initialized(void)
	{
		bool result;

		TRACE_ENTRY();
		SERIALIZE_CALL_RECUR(m_lock);

		result = m_initialized;

		TRACE_EXIT("Return Value: 0x%x", result);
		return result;
	}

	std::string 
	_luna::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		TRACE_ENTRY();
		SERIALIZE_CALL_RECUR(m_lock);

		result << "UUID Factory" << std::endl << "---" << std::endl;

		if(m_uuid_factory) {
			result << m_uuid_factory->to_string(verbose);
		} else {
			result << "(NULL)";
		}

		result << std::endl << "---" << std::endl << "TOKEN Factory" 
			<< std::endl << "---" << std::endl;

		if(m_token_factory) {
			result << m_token_factory->to_string(verbose);
		} else {
			result << "(NULL)";
		}

		result << std::endl << "---" << std::endl << "NODE Factory" 
			<< std::endl << "---" << std::endl;

		if(m_node_factory) {
			result << m_node_factory->to_string(verbose);
		} else {
			result << "(NULL)";
		}

		result << std::endl << "---" << std::endl << "SCOPE Factory" 
			<< std::endl << "---" << std::endl;

		if(m_scope_factory) {
			result << m_scope_factory->to_string(verbose);
		} else {
			result << "(NULL)";
		}

		result << std::endl << "---" << std::endl;

		TRACE_EXIT("Return Value: 0x%x", NULL);
		return result.str();
	}

	std::string 
	_luna::version(
		__in bool verbose,
		__in_opt bool show_title,
		__in_opt bool show_copyright
		)
	{
		std::stringstream result;

		TRACE_ENTRY();

		if(show_title) {
			result << LUNA_TITLE << " ";
		}

		if(verbose) {
			result << LUNA_VER_STR_VERB;
		} else {
			result << LUNA_VER_STR;
		}

		if(show_copyright) {
			result << std::endl << LUNA_COPYRIGHT;
		}

		TRACE_EXIT("Return Value: 0x%x", NULL);
		return result.str();
	}
}
