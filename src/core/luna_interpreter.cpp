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

			// TODO: setup global scope here

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::evaluate_assignment(
			__in node_factory_ptr node_fact,
			__in token_factory_ptr tok_fact,
			__in node &node,
			__in token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			// TODO: evaluate_assignment
			UNREF_PARAM(node_fact);
			UNREF_PARAM(tok_fact);
			UNREF_PARAM(node);
			UNREF_PARAM(tok);
			// ---

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::evaluate_conditional_if(
			__in node_factory_ptr node_fact,
			__in token_factory_ptr tok_fact,
			__in node &node,
			__in token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			// TODO: evaluate_conditional_if
			UNREF_PARAM(node_fact);
			UNREF_PARAM(tok_fact);
			UNREF_PARAM(node);
			UNREF_PARAM(tok);
			// ---

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::evaluate_conditional_while(
			__in node_factory_ptr node_fact,
			__in token_factory_ptr tok_fact,
			__in node &node,
			__in token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			// TODO: evaluate_conditional_while
			UNREF_PARAM(node_fact);
			UNREF_PARAM(tok_fact);
			UNREF_PARAM(node);
			UNREF_PARAM(tok);
			// ---

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::evaluate_control(
			__in node_factory_ptr node_fact,
			__in token_factory_ptr tok_fact,
			__in node &node,
			__in token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			// TODO: evaluate_control
			UNREF_PARAM(node_fact);
			UNREF_PARAM(tok_fact);
			UNREF_PARAM(node);
			UNREF_PARAM(tok);
			// ---

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::evaluate_function_call(
			__in node_factory_ptr node_fact,
			__in token_factory_ptr tok_fact,
			__in node &node,
			__in token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			// TODO: evaluate_function_call
			UNREF_PARAM(node_fact);
			UNREF_PARAM(tok_fact);
			UNREF_PARAM(node);
			UNREF_PARAM(tok);
			// ---

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}
				
		void 
		_interpreter::evaluate_print(
			__in node_factory_ptr node_fact,
			__in token_factory_ptr tok_fact,
			__in node &node,
			__in token &tok
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			// TODO: evaluate_print
			UNREF_PARAM(node_fact);
			UNREF_PARAM(tok_fact);
			UNREF_PARAM(node);
			UNREF_PARAM(tok);
			// ---

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_interpreter::flush_scopes(void)
		{
			scope_factory_ptr scop_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(luna::is_globally_initialized()) {

				scop_fact = get_scope_factory();
				if(scop_fact) {

					while(!m_scope_stack.empty()) {
						scop_fact->remove_scope(m_scope_stack.top());
						pop_scope();
					}
				}
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

			if(luna::is_globally_initialized()) {
				get_scope_factory()->remove_scope(m_scope_stack.top());
			}

			m_scope_stack.pop();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		uuid 
		_interpreter::push_scope(void)
		{
			uuid result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = get_scope_factory()->generate().get_id();
			m_scope_stack.push(result);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
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
		_interpreter::step(
			__in_opt bool expect_next
			)
		{
			node_factory_ptr node_fact;
			token_factory_ptr tok_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!parser::has_next_statement()) {
				THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_NO_NEXT_STATEMENT);
			}

			if(parser::get_statement().front().get_id() == parser::get_begin_statement().front().get_id()) {
				parser::move_next_statement();
			}

			if(parser::has_next_statement()) {
				
				node_fact = parser::get_node_factory();
				if(!node_fact) {
					THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_FACTORY_ALLOC_FAILED);
				}

				tok_fact = lexer::get_token_factory();
				if(!tok_fact) {
					THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_FACTORY_ALLOC_FAILED);
				}

				node &stmt_nd = node_fact->get_node(parser::get_statement().front());
				token &stmt_tok = tok_fact->get_token(stmt_nd.get_token_id());

				if(!stmt_nd.get_child_count()
						|| (stmt_tok.get_type() != TOKEN_STATEMENT)) {
					THROW_LUNA_INTERPRETER_EXCEPTION_MESSAGE(LUNA_INTERPRETER_EXCEPTION_EXPECTING_STATEMENT,
						"%s", stmt_tok.to_string().c_str());
				}

				node &left_child_nd = node_fact->get_node(parser::get_statement().at(stmt_nd.get_child(0)));
				token &left_child_tok = tok_fact->get_token(left_child_nd.get_token_id());

				switch(left_child_tok.get_type()) {
					case TOKEN_CONTROL:
						evaluate_control(node_fact, tok_fact, stmt_nd, stmt_tok);
						break;
					case TOKEN_IDENTIFIER: {
							node &right_child_nd = node_fact->get_node(parser::get_statement().at(stmt_nd.get_child(1)));
							token &right_child_tok = tok_fact->get_token(right_child_nd.get_token_id());

							switch(right_child_tok.get_type()) {
								case TOKEN_ASSIGNMENT:
									evaluate_assignment(node_fact, tok_fact, stmt_nd, stmt_tok);
									break;
								case TOKEN_FUNCTION_CALL:
									evaluate_function_call(node_fact, tok_fact, stmt_nd, stmt_tok);
									break;
								default:
									THROW_LUNA_INTERPRETER_EXCEPTION_MESSAGE(LUNA_INTERPRETER_EXCEPTION_EXPECTING_ASSIGNMENT,
										"%s", right_child_tok.to_string().c_str());
							}
						} break;
					case TOKEN_KEYWORD:

						switch(left_child_tok.get_subtype()) {
						case KEYWORD_IF:
							evaluate_conditional_if(node_fact, tok_fact, stmt_nd, stmt_tok);
							break;
						case KEYWORD_WHILE:
							evaluate_conditional_while(node_fact, tok_fact, stmt_nd, stmt_tok);
							break;
						case KEYWORD_PRINT:
							evaluate_print(node_fact, tok_fact, stmt_nd, stmt_tok);
							break;
						default:
							THROW_LUNA_INTERPRETER_EXCEPTION_MESSAGE(LUNA_INTERPRETER_EXCEPTION_EXPECTING_CONDITIONAL,
								"%s", left_child_tok.to_string().c_str());
						}
						break;
					default:
						THROW_LUNA_INTERPRETER_EXCEPTION_MESSAGE(LUNA_INTERPRETER_EXCEPTION_EXPECTING_STATEMENT,
							"%s", left_child_tok.to_string().c_str());
				}

				if(parser::has_next_statement()) {
					parser::move_next_statement();
				} else if(expect_next) {
					THROW_LUNA_INTERPRETER_EXCEPTION(LUNA_INTERPRETER_EXCEPTION_EXPECTING_NEXT_STATEMENT);
				}
			}

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
