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
#include "luna_interpreter_type.h"

namespace LUNA_NS {

	namespace LUNA_RUNTIME_NS {
	
		_interpreter::_interpreter(void)
		{
			TRACE_ENTRY();

			interpreter::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_interpreter::_interpreter(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();

			interpreter::set(input, is_file);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_interpreter::_interpreter(
			__in const _interpreter &other
			) :
				parser(other),
				m_scope_stack(other.m_scope_stack)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_interpreter::~_interpreter(void)
		{
			TRACE_ENTRY();

			interpreter::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_interpreter &
		_interpreter::operator=(
			__in const _interpreter &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				parser::operator=(other);
				interpreter::clear();
				m_scope_stack = other.m_scope_stack;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_interpreter::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			parser::reset();
			flush_scopes();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::flush_scopes(void)
		{
			scope_factory_ptr scop_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			scop_fact = get_scope_factory();
			if(!scop_fact) {
				THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_FACTORY_ALLOC_FAILED);
			}

			while(!m_scope_stack.empty()) {
				scop_fact->remove_scope(m_scope_stack.top());
				pop_scope();
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		scope_factory_ptr 
		_interpreter::get_scope_factory(void)
		{
			scope_factory_ptr result = NULL;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(luna::is_globally_initialized()) {
				
				if(get_context()->is_initialized()) {
					result = get_context()->acquire_scope_factory();
				}
			}

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_interpreter::pop_scope(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_scope_stack.empty()) {
				THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_NO_SCOPE);
			}

			m_scope_stack.pop();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::run(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			parser::clear();

			while(parser::has_next_statement()) {
				step();
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			parser::set(input, is_file);
			interpreter::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::step(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!parser::has_next_statement()) {
				THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_NO_NEXT_STATEMENT);
			}

			// TODO: step

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_interpreter::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			scope_factory_ptr scop_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			scop_fact = get_scope_factory();
			if(!scop_fact) {
				THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_FACTORY_ALLOC_FAILED);
			}

			result << parser::to_string(verbose) << std::endl << "---" << std::endl 
				<< scop_fact->get_scope(m_scope_stack.top()).to_string(verbose);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}
