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

#ifndef LUNA_PARSER_H_
#define LUNA_PARSER_H_

namespace LUNA_NS {

	using namespace LUNA_COMP_NS;

	namespace LUNA_LANG_NS {
	
		typedef std::vector<uuid> statement_t, *statement_ptr_t;

		typedef class _parser:
				protected lexer {
		
			public:

				_parser(void);

				_parser(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				_parser(
					__in const _parser &other
					);

				virtual ~_parser(void);

				_parser &operator=(
					__in const _parser &other
					);

				virtual void clear(void);

				virtual size_t discover(void);

				statement_t &get_begin_statement(void);

				statement_t &get_end_statement(void);

				statement_t &get_statement(void);

				statement_t &get_statement(
					__in size_t position
					);

				size_t get_statement_position(void);

				bool has_next_statement(void);

				bool has_previous_statement(void);

				statement_t &move_next_statement(
					__in_opt bool expect_next = false
					);

				statement_t &move_previous_statement(
					__in_opt bool expect_next = false
					);

				virtual void reset(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				size_t add_child_node(
					__in statement_t &stmt,
					__in const uuid &token_id,
					__in_opt size_t parent = PARENT_NODE_NONE
					);
				
				size_t add_control_node(
					__in statement_t &stmt,
					__in token_t type,
					__in_opt size_t parent = PARENT_NODE_NONE
					);

				statement_t &add_control_node(
					__in token_t type,
					__in_opt size_t position = 0
					);

				void add_root_node(
					__in const uuid &node_id,
					__in size_t position
					);

				uuid create_node(
					__in const uuid &token_id,
					__in_opt size_t parent = PARENT_NODE_NONE,
					__in_opt const std::vector<size_t> &children = std::vector<size_t>()
					);

				size_t enumerate_assignment(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_assignment_list(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_call(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_conditional_else(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_conditional_if(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_condition_list(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_conditional_statement(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_conditional_while(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_control(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_array_list(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_compound(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_function(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_list(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_0p(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_1(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_1p(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_2(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_2p(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_3(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_3p(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_4(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_4p(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_5(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_5p(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_numeric_6(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_string(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_expression_struct(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_identifier_list(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_identifier_reference(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				size_t enumerate_statement(
					__in statement_t &stmt,
					__in_opt size_t parent = 0,
					__in_opt bool root = false
					);

				size_t enumerate_statement_list(
					__in statement_t &stmt,
					__in_opt size_t parent = 0
					);

				node_factory_ptr get_node_factory(void);

				std::string statement_as_string(
					__in const statement_t &stmt,
					__in_opt bool verbose = false
					);

				std::vector<uuid> m_control_token_list;

				std::vector<statement_t> m_statement_list;

				size_t m_statement_position;

			private:

				std::recursive_mutex m_lock;

		} parser, *parser_ptr;
	}
}

#endif // LUNA_PARSER_H_
