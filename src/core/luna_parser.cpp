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

#include <queue>
#include "luna.h"
#include "luna_parser_type.h"

namespace LUNA_NS {

	namespace LUNA_LANG_NS {
	
		#define NODE_TAB_SEQUENCE "---"

		_parser::_parser(void) :
			m_statement_position(0)
		{
			TRACE_ENTRY();

			parser::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_parser::_parser(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();

			parser::set(input, is_file);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_parser::_parser(
			__in const _parser &other
			) :
				lexer(other),
				m_control_token_list(other.m_control_token_list),
				m_statement_list(other.m_statement_list),
				m_statement_position(other.m_statement_position)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_parser::~_parser(void)
		{
			TRACE_ENTRY();

			parser::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_parser &
		_parser::operator=(
			__in const _parser &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				lexer::operator=(other);
				parser::clear();
				m_control_token_list = other.m_control_token_list;
				m_statement_list = other.m_statement_list;
				m_statement_position = other.m_statement_position;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		size_t 
		_parser::add_child_node(
			__in statement_t &stmt,
			__in const uuid &token_id,
			__in_opt size_t parent
			)
		{
			size_t result;
			node_factory_ptr node_fact;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(parent >= stmt.size()) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_INVALID_PARENT_STATEMENT_POSITION,
					"pos/max: %lu/%lu", parent, (stmt.size() - 1));
			}

			node_fact = get_node_factory();
			if(!node_fact) {
				THROW_LUNA_PARSER_EXCEPTION(LUNA_PARSER_EXCEPTION_FACTORY_ALLOC_FAILED);
			}

			result = stmt.size();
			stmt.push_back(create_node(token_id, parent));
			node_fact->get_node(stmt.at(parent)).add_child(result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::add_control_node(
			__in statement_t &stmt,
			__in token_t type,
			__in_opt size_t parent
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_control_token_list.push_back(lexer::create_token(type));
			result = add_child_node(stmt, m_control_token_list.back(), parent);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		statement_t & 
		_parser::add_control_node(
			__in token_t type,
			__in_opt size_t position
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_control_token_list.push_back(lexer::create_token(type));
			add_root_node(create_node(m_control_token_list.back()), position);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_statement_list.at(m_statement_position + 1);
		}

		void 
		_parser::add_root_node(
			__in const uuid &node_id,
			__in size_t position
			)
		{
			statement_t stmt;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position > m_statement_list.size()) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_INVALID_STATEMENT_POSITION, 
					"pos/max: %lu/%lu", position, m_statement_list.size());
			}

			stmt.push_back(node_id);

			if(position < m_statement_list.size()) {
				m_statement_list.insert(m_statement_list.begin() + position, stmt);
			} else {
				m_statement_list.push_back(stmt);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_parser::clear(void)
		{
			node_factory_ptr node_fact;
			token_factory_ptr tok_fact;
			std::vector<uuid>::iterator id_iter;
			std::vector<statement_t>::iterator stmt_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			lexer::reset();

			tok_fact = lexer::get_token_factory();
			if(tok_fact) {
				
				for(id_iter = m_control_token_list.begin(); id_iter != m_control_token_list.end(); ++id_iter) {
					tok_fact->remove_token(*id_iter);
				}
			}

			node_fact = get_node_factory();
			if(node_fact) {

				for(stmt_iter = m_statement_list.begin(); stmt_iter != m_statement_list.end(); ++stmt_iter) {

					for(id_iter = stmt_iter->begin(); id_iter != stmt_iter->end(); ++id_iter) {
						node_fact->remove_node(*id_iter);
					}
				}
			}

			m_statement_list.clear();

			if(node_fact) {
				add_root_node(create_node(lexer::get_begin_token().get_id()), 0);
				add_root_node(create_node(lexer::get_end_token().get_id()), 1);
			}

			m_statement_position = 0;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		uuid 
		_parser::create_node(
			__in const uuid &token_id,
			__in_opt size_t parent,
			__in_opt const std::vector<size_t> &children
			)
		{
			node_factory_ptr node_fact;
			std::vector<size_t>::const_iterator child_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			node_fact = get_node_factory();
			if(!node_fact) {
				THROW_LUNA_PARSER_EXCEPTION(LUNA_PARSER_EXCEPTION_FACTORY_ALLOC_FAILED);
			}

			node &nd = node_fact->generate(token_id, parent);
			
			for(child_iter = children.begin(); child_iter != children.end(); ++child_iter) {
				nd.add_child(*child_iter);
			}

			TRACE_EXIT("Return Value: %s", nd.to_string().c_str());
			return nd.get_id();
		}

		size_t 
		_parser::discover(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			parser::reset();

			while(parser::has_next_statement()) {
				parser::move_next_statement();
			}

			result = parser::size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

size_t 
		_parser::enumerate_assignment(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer::get_token().get_type() != TOKEN_IDENTIFIER) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
					"%s", lexer::to_string().c_str());
			}

			enumerate_identifier_reference(stmt, parent);

			if(lexer::get_token().get_type() != TOKEN_SYMBOL) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_ASSIGNMENT,
					"%s", lexer::to_string().c_str());
			}

			switch(lexer::get_token().get_subtype()) {
				case SYMBOL_ASSIGNMENT:
					result = add_control_node(stmt, TOKEN_ASSIGNMENT, result);

					if(!lexer::has_next_token()) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
							"%s", lexer::to_string().c_str());
					}

					lexer::move_next_token();
					enumerate_expression(stmt, result);
					break;
				case SYMBOL_PARENTHESIS_OPEN:
					result = add_control_node(stmt, TOKEN_FUNCTION_CALL, result);

					if(!lexer::has_next_token()) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
							"%s", lexer::to_string().c_str());
					}

					lexer::move_next_token();

					if((lexer::get_token().get_type() != TOKEN_SYMBOL)
							|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
						enumerate_expression_list(stmt, result);

						if((lexer::get_token().get_type() != TOKEN_SYMBOL)
								|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
							THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
								"%s", lexer::to_string().c_str());
						}
					}

					if(lexer::has_next_token()) {
						lexer::move_next_token();
					}
					break;
				default:
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_ASSIGNMENT,
						"%s", lexer::to_string().c_str());
			}

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_TERMINATOR)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_TERMINATOR,
					"%s", lexer::to_string().c_str());
			}

			if(lexer::has_next_token()) {
				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_assignment_list(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t list_loc, result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_ASSIGNMENT_LIST, parent);
			list_loc = result;

			for(;;) {
				result = list_loc;

				if(lexer::get_token().get_type() != TOKEN_IDENTIFIER) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
						"%s", lexer::to_string().c_str());
				}

				result = enumerate_identifier_reference(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_ASSIGNMENT)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_ASSIGNMENT,
						"%s", lexer::to_string().c_str());
				}

				result = add_control_node(stmt, TOKEN_ASSIGNMENT, result);

				if(!lexer::has_next_token()) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
				enumerate_expression(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_SEPERATOR)) {
					break;
				}

				if(!lexer::has_next_token()) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_ASSIGNMENT,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_call(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer::get_token().get_type() != TOKEN_KEYWORD) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_KEYWORD,
					"%s", lexer::to_string().c_str());
			}

			switch(lexer::get_token().get_subtype()) {
				case KEYWORD_PRINT:

					result = add_child_node(stmt, lexer::get_token().get_id(), parent);
					lexer::move_next_token();

					if((lexer::get_token().get_type() != TOKEN_SYMBOL)
							|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_OPEN)) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_PARENTHESIS,
							"%s", lexer::to_string().c_str());
					}

					lexer::move_next_token();
					enumerate_expression(stmt, result);

					if((lexer::get_token().get_type() != TOKEN_SYMBOL)
							|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
							"%s", lexer::to_string().c_str());
					}

					lexer::move_next_token();

					if((lexer::get_token().get_type() != TOKEN_SYMBOL)
							|| (lexer::get_token().get_subtype() != SYMBOL_TERMINATOR)) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_TERMINATOR,
							"%s", lexer::to_string().c_str());
					}

					if(lexer::has_next_token()) {
						lexer::move_next_token();
					}
					break;
				default:
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_GENERIC_CALL,
						"%s", lexer::to_string().c_str());
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_conditional_else(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() != TOKEN_KEYWORD)
					|| (lexer::get_token().get_subtype() != KEYWORD_ELSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_ELSE_CONDITIONAL,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();
	
			if((lexer::get_token().get_type() == TOKEN_KEYWORD)
					&& (lexer::get_token().get_subtype() == KEYWORD_IF)) {
				enumerate_conditional_if(stmt, parent);
			} else {
				lexer::move_previous_token();
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_OPEN)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
				enumerate_statement_list(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_CLOSE)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
						"%s", lexer::to_string().c_str());
				}

				if(lexer::has_next_token()) {
					lexer::move_next_token();
				}
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_conditional_if(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() != TOKEN_KEYWORD)
					|| (lexer::get_token().get_subtype() != KEYWORD_IF)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_IF_CONDITIONAL,
					"%s", lexer::to_string().c_str());
			}

			result = add_child_node(stmt, lexer::get_token().get_id(), result);
			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_PARENTHESIS,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();
			enumerate_condition_list(stmt, result);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();
			enumerate_statement_list(stmt, result);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_CLOSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			if(lexer::has_next_token()) {
				lexer::move_next_token();

				while((lexer::get_token().get_type() == TOKEN_KEYWORD)
						&& (lexer::get_token().get_subtype() == KEYWORD_ELSE)) {

					if(!lexer::has_next_token()) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_ELSE_CONDITIONAL,
							"%s", lexer::to_string().c_str());
					}

					enumerate_conditional_else(stmt, parent);
				}
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_condition_list(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_CONDITIONAL_LIST, result);

			for(;;) {
				enumerate_expression_compound(stmt, result);

				if(lexer::get_token().get_type() != TOKEN_OPERATOR) {
					break;
				}

				add_child_node(stmt, lexer::get_token().get_id(), result);

				if(!lexer::has_next_token()) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CONDITIONAL_EXPRESSION,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_conditional_statement(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer::get_token().get_type() != TOKEN_KEYWORD) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_KEYWORD,
					"%s", lexer::to_string().c_str());
			}

			switch(lexer::get_token().get_subtype()) {
				case KEYWORD_IF:
					enumerate_conditional_if(stmt, result);
					break;
				case KEYWORD_WHILE:
					enumerate_conditional_while(stmt, result);
					break;
				default:
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CONDITIONAL_STATEMENT,
						"%s", lexer::to_string().c_str());
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_conditional_while(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_child_node(stmt, lexer::get_token().get_id(), result);
			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_PARENTHESIS,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();
			enumerate_condition_list(stmt, result);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();
			enumerate_statement_list(stmt, result);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_CLOSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			if(lexer::has_next_token()) {
				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_control(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer::get_token().get_type() != TOKEN_CONTROL) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CONTROL,
					"%s", lexer::to_string().c_str());
			}

			result = add_child_node(stmt, lexer::get_token().get_id(), result);
			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_TERMINATOR)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_TERMINATOR,
					"%s", lexer::to_string().c_str());
			}

			if(lexer::has_next_token()) {
				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_EXPRESSION, parent);

			switch(lexer::get_token().get_type()) {
				case TOKEN_KEYWORD:

					if(lexer::get_token().get_subtype() == KEYWORD_FUNCT) {
						enumerate_expression_function(stmt, result);
					} else {
						enumerate_expression_struct(stmt, result);
					}
					break;
				case TOKEN_SYMBOL:

					switch(lexer::get_token().get_subtype()) {
						case SYMBOL_NEGATIVE:
						case SYMBOL_PARENTHESIS_OPEN:
							enumerate_expression_compound(stmt, result);
							break;
						default:
							enumerate_expression_array_list(stmt, result);
							break;
					}
					break;
				default:
					enumerate_expression_compound(stmt, result);
					break;
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_array_list(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			result = add_control_node(stmt, TOKEN_ARRAY_LIST, parent);
			lexer::move_next_token();
			enumerate_expression_list(stmt, result);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_CLOSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			if(lexer::has_next_token()) {
				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_compound(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			token_t type;
			size_t nest_count = 0, result = parent;
			
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_EXPRESSION_COMPOUND, result);

			if((lexer::get_token().get_type() == TOKEN_SYMBOL)
					&& (lexer::get_token().get_subtype() == SYMBOL_PARENTHESIS_OPEN)) {

				while((lexer::get_token().get_type() == TOKEN_SYMBOL)
						&& (lexer::get_token().get_subtype() == SYMBOL_PARENTHESIS_OPEN)) {
					lexer::move_next_token();
					++nest_count;
				}

				type = lexer::get_token().get_type();

				while(nest_count--) {
					lexer::move_previous_token();
				}

				if(type == TOKEN_LITERAL_STRING) {
					result = enumerate_expression_string(stmt, result);
				} else {
					result = enumerate_expression_numeric(stmt, result);
				}
			} else if(lexer::get_token().get_type() == TOKEN_LITERAL_STRING) {
				result = enumerate_expression_string(stmt, result);
			} else {
				result = enumerate_expression_numeric(stmt, result);
			}

			switch(lexer::get_token().get_type()) {
			case TOKEN_LITERAL_STRING:
				break;
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_function(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() != TOKEN_KEYWORD)
					|| (lexer::get_token().get_subtype() != KEYWORD_FUNCT)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_FUNCTION,
					"%s", lexer::to_string().c_str());
			}

			result = add_child_node(stmt, lexer::get_token().get_id(), result);
			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_PARENTHESIS,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
				enumerate_identifier_list(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
						"%s", lexer::to_string().c_str());
				}
			}

			lexer::move_next_token();

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_OPEN)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			lexer::move_next_token();
			enumerate_statement_list(stmt, parent);

			if((lexer::get_token().get_type() != TOKEN_SYMBOL)
					|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_CLOSE)) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
					"%s", lexer::to_string().c_str());
			}

			if(lexer::has_next_token()) {
				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_list(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_EXPRESSION_LIST, parent);

			for(;;) {
				enumerate_expression(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_SEPERATOR)) {
					break;
				}

				if(!lexer::has_next_token()) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			enumerate_expression_numeric_1(stmt, result);
			enumerate_expression_numeric_0p(stmt, result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_0p(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer::get_token().get_type() == TOKEN_BINARY_OPERATOR) {
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();
				enumerate_expression_numeric_1(stmt, result);
				enumerate_expression_numeric_0p(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_1(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			enumerate_expression_numeric_2(stmt, result);
			enumerate_expression_numeric_1p(stmt, result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_1p(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(lexer::get_token().get_type() == TOKEN_SHIFT_OPEATOR) {
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();
				enumerate_expression_numeric_2(stmt, result);
				enumerate_expression_numeric_1p(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_2(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			enumerate_expression_numeric_3(stmt, result);
			enumerate_expression_numeric_2p(stmt, result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_2p(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() == TOKEN_ARITHMETIC_OPERATOR)
					&& (lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_SUBTRACTION)) {
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();
				enumerate_expression_numeric_3(stmt, result);
				enumerate_expression_numeric_2p(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_3(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			enumerate_expression_numeric_4(stmt, result);
			enumerate_expression_numeric_3p(stmt, result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_3p(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() == TOKEN_ARITHMETIC_OPERATOR)
					&& (lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_ADDITION)) {
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();
				enumerate_expression_numeric_4(stmt, result);
				enumerate_expression_numeric_3p(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_4(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			enumerate_expression_numeric_5(stmt, result);
			enumerate_expression_numeric_4p(stmt, result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_4p(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() == TOKEN_ARITHMETIC_OPERATOR)
					&& ((lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_DIVISION)
					|| (lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_MODULUS))) {
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();
				enumerate_expression_numeric_5(stmt, result);
				enumerate_expression_numeric_4p(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_5(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			enumerate_expression_numeric_6(stmt, result);
			enumerate_expression_numeric_5p(stmt, result);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_5p(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() == TOKEN_ARITHMETIC_OPERATOR)
					&& (lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_MULTIPLICATION)) {
				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();
				enumerate_expression_numeric_6(stmt, result);
				enumerate_expression_numeric_5p(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_numeric_6(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			switch(lexer::get_token().get_type()) {
				case TOKEN_IDENTIFIER:
					result = enumerate_identifier_reference(stmt, result);
					break;
				case TOKEN_BOOLEAN:
				case TOKEN_FLOAT:
				case TOKEN_LITERAL_CHARACTER:
				case TOKEN_NULL:
				case TOKEN_SCALAR:
					result = add_child_node(stmt, lexer::get_token().get_id(), result);
					lexer::move_next_token();
					break;
				case TOKEN_SYMBOL:

					switch(lexer::get_token().get_subtype()) {
						case SYMBOL_PARENTHESIS_OPEN:
							lexer::move_next_token();
							enumerate_expression_numeric(stmt, result);

							if((lexer::get_token().get_type() != TOKEN_SYMBOL)
									|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
								THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
									"%s", lexer::to_string().c_str());
							}

							if(lexer::has_next_token()) {
								lexer::move_next_token();
							}
							break;
						case SYMBOL_NEGATIVE:
							result = add_child_node(stmt, lexer::get_token().get_id(), result);
							lexer::move_next_token();
							enumerate_expression_numeric(stmt, result);
							break;
						default:
							THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION_NUMERIC,
								"%s", lexer::to_string().c_str());
					}
					break;
				default:
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_EXPRESSION_NUMERIC,
						"%s", lexer::to_string().c_str());
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_expression_string(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if((lexer::get_token().get_type() == TOKEN_SYMBOL)
						&& (lexer::get_token().get_subtype() == SYMBOL_PARENTHESIS_OPEN)) {
				lexer::move_next_token();
				enumerate_expression_string(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_PARENTHESIS_CLOSE)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_PARENTHESIS,
						"%s", lexer::to_string().c_str());
				}

				if(lexer::has_next_token()) {
					lexer::move_next_token();
				}

				if((lexer::get_token().get_type() == TOKEN_ARITHMETIC_OPERATOR)
							&& (lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_ADDITION)) {
					result = add_child_node(stmt, lexer::get_token().get_id(), result);
					lexer::move_next_token();
					enumerate_expression_compound(stmt, result);
				}
			} else if(lexer::get_token().get_type() != TOKEN_LITERAL_STRING) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_STRING,
					"%s", lexer::to_string().c_str());
			} else {
				add_child_node(stmt, lexer::get_token().get_id(), result);
			
				if(lexer::has_next_token()) {
					lexer::move_next_token();
				}

				if((lexer::get_token().get_type() == TOKEN_ARITHMETIC_OPERATOR)
							&& (lexer::get_token().get_subtype() == ARITHMETIC_OPERATOR_ADDITION)) {
					result = add_child_node(stmt, lexer::get_token().get_id(), result);
					lexer::move_next_token();
					enumerate_expression_compound(stmt, result);
				}
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;		
		}

		size_t 
		_parser::enumerate_expression_struct(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
				size_t result = parent;

				TRACE_ENTRY();
				SERIALIZE_CALL_RECUR(m_lock);

				if((lexer::get_token().get_type() != TOKEN_KEYWORD)
						|| (lexer::get_token().get_subtype() != KEYWORD_STRUCT)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_STRUCT,
						"%s", lexer::to_string().c_str());
				}

				result = add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_OPEN)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_OPENING_BRACE,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
				enumerate_assignment_list(stmt, result);

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_CURLY_BRACE_CLOSE)) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACE,
						"%s", lexer::to_string().c_str());
				}

				if(lexer::has_next_token()) {
					lexer::move_next_token();
				}

				TRACE_EXIT("Return Value: %lu", result);
				return result;
		}

		size_t 
		_parser::enumerate_identifier_list(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_IDENTIFIER_LIST, parent);

			for(;;) {

				if(lexer::get_token().get_type() != TOKEN_IDENTIFIER) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
						"%s", lexer::to_string().c_str());
				}

				add_child_node(stmt, lexer::get_token().get_id(), result);
				lexer::move_next_token();

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_SEPERATOR)) {
					break;
				}

				if(!lexer::has_next_token()) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
						"%s", lexer::to_string().c_str());
				}

				lexer::move_next_token();
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_identifier_reference(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			for(;;) {

				if(lexer::get_token().get_type() != TOKEN_IDENTIFIER) {
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
						"%s", lexer::to_string().c_str());
				}

				result = add_child_node(stmt, lexer::get_token().get_id(), result);

				if(!lexer::has_next_token()) {
					break;
				}

				lexer::move_next_token();

				if((lexer::get_token().get_type() != TOKEN_SYMBOL)
						|| (lexer::get_token().get_subtype() != SYMBOL_REFERENCE)) {
					break;
				}

				lexer::move_next_token(true);
			}

			if(lexer::has_next_token()) {
				
				if((lexer::get_token().get_type() == TOKEN_SYMBOL) 
						&& (lexer::get_token().get_subtype() == SYMBOL_SQUARE_BRACKET_OPEN)) {
					lexer::move_next_token(true);
					result = add_control_node(stmt, TOKEN_ARRAY_INDEX, result);

					switch(lexer::get_token().get_type()) {
						case TOKEN_IDENTIFIER:
							result = enumerate_identifier_reference(stmt, result);
							break;
						case TOKEN_SCALAR:
							result = add_child_node(stmt, lexer::get_token().get_id(), result);

							if(!lexer::has_next_token()) {
								THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACKET,
									"%s", lexer::to_string().c_str());
							}

							lexer::move_next_token(true);
							break;
						default:
							THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_SCALAR,
								"%s", lexer::to_string().c_str());
					}

					if((lexer::get_token().get_type() != TOKEN_SYMBOL)
							|| (lexer::get_token().get_subtype() != SYMBOL_SQUARE_BRACKET_CLOSE)) {
						THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_CLOSING_BRACKET,
							"%s", lexer::to_string().c_str());
					}

					if(lexer::has_next_token()) {
						lexer::move_next_token();
					}
				}
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_statement(
			__in statement_t &stmt,
			__in_opt size_t parent,
			__in_opt bool root
			)
		{
			uuid node_id;
			size_t result = parent;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!root) {
				result = add_control_node(stmt, TOKEN_STATEMENT, result);
			}

			switch(lexer::get_token().get_type()) {
				case TOKEN_CONTROL:
					enumerate_control(stmt, result);
					break;
				case TOKEN_IDENTIFIER:
					enumerate_assignment(stmt, result);
					break;
				case TOKEN_KEYWORD:
					
					switch(lexer::get_token().get_subtype()) {
						case KEYWORD_PRINT:
							enumerate_call(stmt, result);
							break;
						default:
							enumerate_conditional_statement(stmt, result);
							break;
					}
					break;
				default:
					THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_EXPECTING_STATEMENT,
						"%s", lexer::to_string().c_str());
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_parser::enumerate_statement_list(
			__in statement_t &stmt,
			__in_opt size_t parent
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_control_node(stmt, TOKEN_STATEMENT_LIST, parent);

			for(;;) {
				
				if((lexer::get_token().get_type() != TOKEN_CONTROL)
						&& (lexer::get_token().get_type() != TOKEN_IDENTIFIER)
						&& (lexer::get_token().get_type() != TOKEN_KEYWORD)) {
					break;
				}

				enumerate_statement(stmt, result);
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		statement_t &
		_parser::get_begin_statement(void)
		{
			return get_statement(0);
		}

		statement_t &
		_parser::get_end_statement(void)
		{
			return get_statement(m_statement_list.size() - 1);
		}

		node_factory_ptr 
		_parser::get_node_factory(void)
		{
			node_factory_ptr result = NULL;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(luna::is_globally_initialized()) {
				
				if(get_context()->is_initialized()) {
					result = get_context()->acquire_node_factory();
				}
			}

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		statement_t &
		_parser::get_statement(void)
		{
			return get_statement(m_statement_position);
		}

		statement_t &
		_parser::get_statement(
			__in size_t position
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position >= m_statement_list.size()) {
				THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_INVALID_STATEMENT_POSITION,
					"pos/max: %lu/%lu", position, (m_statement_list.size() - 1));
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_statement_list.at(position);
		}

		size_t 
		_parser::get_statement_position(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_statement_position);
			return m_statement_position;
		}

		bool 
		_parser::has_next_statement(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (get_statement().front().get_id() != get_end_statement().front().get_id());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_parser::has_previous_statement(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_statement_position > 0);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		statement_t &
		_parser::move_next_statement(
			__in_opt bool expect_next
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!has_next_statement()) {
				THROW_LUNA_PARSER_EXCEPTION(expect_next ? LUNA_PARSER_EXCEPTION_EXPECTING_NEXT_STATEMENT 
					: LUNA_PARSER_EXCEPTION_NO_NEXT_STATEMENT);
			}

			if(lexer::get_token().get_id() == lexer::get_begin_token().get_id()) {
				move_next_token();
			}

			if(lexer::has_next_token() 
					&& (m_statement_position == (m_statement_list.size() - SENTINEL_COUNT))) {
				statement_t &stmt = add_control_node(TOKEN_STATEMENT, m_statement_position + 1);
				enumerate_statement(stmt, 0, true);
			}

			++m_statement_position;

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_statement();
		}

		statement_t &
		_parser::move_previous_statement(
			__in_opt bool expect_next
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!has_previous_statement()) {
				THROW_LUNA_PARSER_EXCEPTION(expect_next ? LUNA_PARSER_EXCEPTION_EXPECTING_PREVIOUS_STATEMENT 
					: LUNA_PARSER_EXCEPTION_NO_PREVIOUS_STATEMENT);
			}

			--m_statement_position;

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_statement();
		}

		void 
		_parser::reset(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_statement_position = 0;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_parser::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			lexer::set(input, is_file);
			parser::clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_parser::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_statement_list.size() - SENTINEL_COUNT);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_parser::statement_as_string(
			__in const statement_t &stmt,
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			node_factory_ptr node_fact;
			token_factory_ptr tok_fact;
			statement_t::const_iterator stmt_iter;
			std::queue<std::pair<uuid, size_t>> node_que;
			size_t child_pos, pos_iter, tab_count, tab_iter;

			TRACE_ENTRY();

			if(!stmt.empty()) {
				
				tok_fact = lexer::get_token_factory();
				if(!tok_fact) {
					THROW_LUNA_PARSER_EXCEPTION(LUNA_PARSER_EXCEPTION_FACTORY_ALLOC_FAILED);
				}

				node_fact = get_node_factory();
				if(!node_fact) {
					THROW_LUNA_PARSER_EXCEPTION(LUNA_PARSER_EXCEPTION_FACTORY_ALLOC_FAILED);
				}

				node_que.push(std::pair<uuid, size_t>(stmt.front(), 0));

				while(!node_que.empty()) {
					
					tab_count = node_que.front().second;
					if(tab_count) {
						result << std::endl;
					}

					for(tab_iter = 0; tab_iter < tab_count; ++tab_iter) {
						result << NODE_TAB_SEQUENCE;
					}

					if(tab_count) {
						result << " ";
					}

					node &nd = node_fact->get_node(node_que.front().first);
					result << tok_fact->get_token(nd.get_token_id()).to_string(verbose) << " [" << nd.get_child_count() << "]";

					if(nd.is_leaf() || nd.is_root()) {
						result << " (";

						if(nd.is_root()) {
							result << "ROOT";

							if(nd.is_leaf()) {
								result << ", LEAF";
							}
						} else {
							result << "LEAF";
						}

						result << ")";
					}

					for(pos_iter = 0; pos_iter < nd.get_child_count(); ++pos_iter) {
						child_pos = nd.get_child(pos_iter);

						if(child_pos >= stmt.size()) {
							THROW_LUNA_PARSER_EXCEPTION_MESSAGE(LUNA_PARSER_EXCEPTION_INVALID_CHILD_STATEMENT_POSITION,
								"pos/max: %lu/%lu", child_pos, (stmt.size() - 1));
						}

						node_que.push(std::pair<uuid, size_t>(stmt.at(child_pos), tab_count + 1));
					}

					--tab_count;
					node_que.pop();
				}
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		std::string 
		_parser::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << "(" << m_statement_position << "/" << (m_statement_list.size() - 1) << ") ";
			}

			result << statement_as_string(get_statement(), verbose);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}
