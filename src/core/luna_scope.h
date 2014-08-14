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

#ifndef LUNA_SCOPE_H_
#define LUNA_SCOPE_H_

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		typedef enum {
			SCOPE_SYMBOL_ARRAY = 0,
			SCOPE_SYMBOL_BOOLEAN,
			SCOPE_SYMBOL_FLOAT,
			SCOPE_SYMBOL_FUNCTION,
			SCOPE_SYMBOL_IDENTIFIER,
			SCOPE_SYMBOL_INTEGER,
			SCOPE_SYMBOL_LITERAL_STRING,
			SCOPE_SYMBOL_NULL,
			SCOPE_SYMBOL_STRUCTURE,
		} scope_sym_t;

		#define SCOPE_SYMBOL_MAX SCOPE_SYMBOL_STRUCTURE

		class _scope;

		typedef class _symbol :
				public uuid_base {
		
			public:

				_symbol(
					__in _scope *parent,
					__in const std::string &key,
					__in scope_sym_t type
					);

				_symbol(
					__in const _symbol &other
					);

				virtual ~_symbol(void);

				_symbol &operator=(
					__in const _symbol &other
					);

				std::string &get_key(void);

				scope_sym_t get_type(void);

				void set_key(
					__in const std::string &key
					);

				void set_type(
					__in scope_sym_t type
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::string m_key;

				_scope *m_parent;

				scope_sym_t m_type;

				std::recursive_mutex m_lock;

		} symbol, *symbol_ptr;

		typedef class _symbol_array :
				public symbol {
		
			public:

				_symbol_array(
					__in _scope *parent,
					__in const std::string &key
					);

				_symbol_array(
					__in _symbol_array &other
					);

				virtual ~_symbol_array(void);

				_symbol_array &operator=(
					__in _symbol_array &other
					);

				void add_value(
					__in const uuid &value
					);

				void clear(void);

				uuid get_value(
					__in size_t index
					);

				void insert_value(
					__in const uuid &value,
					__in size_t index
					);

				void remove_value(
					__in size_t index
					);

				void set_value(
					__in const uuid &value,
					__in size_t index
					);

				virtual size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::vector<uuid> m_value;

		} symbol_array, *symbol_array_ptr;

		typedef class _symbol_boolean :
				public symbol {
		
			public:

				_symbol_boolean(
					__in _scope *parent,
					__in const std::string &key,
					__in_opt bool value = false
					);

				_symbol_boolean(
					__in _symbol_boolean &other
					);

				virtual ~_symbol_boolean(void);

				_symbol_boolean &operator=(
					__in _symbol_boolean &other
					);

				bool get_value(void);

				void set_value(
					__in bool value
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				bool m_value;

		} symbol_boolean, *symbol_boolean_ptr;

		typedef class _symbol_float :
				public symbol {
		
			public:

				_symbol_float(
					__in _scope *parent,
					__in const std::string &key,
					__in_opt double value = 0
					);

				_symbol_float(
					__in _symbol_float &other
					);

				virtual ~_symbol_float(void);

				_symbol_float &operator=(
					__in _symbol_float &other
					);

				double get_value(void);

				void set_value(
					__in double value
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				double m_value;

		} symbol_float, *symbol_float_ptr;

		typedef class _symbol_function :
				public symbol {
		
			public:

				_symbol_function(
					__in _scope *parent,
					__in const std::string &key
					);

				_symbol_function(
					__in _symbol_function &other
					);

				virtual ~_symbol_function(void);

				_symbol_function &operator=(
					__in _symbol_function &other
					);

				void clear(void);

				std::string get_paramter(
					__in size_t index
					);

				size_t get_paramter_count(void);

				uuid get_statement(
					__in size_t index
					);

				size_t get_statement_count(void);

				void set_parameters(
					__in const std::vector<std::string> &parameters
					);

				void set_statements(
					__in const std::vector<uuid> &statements
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::vector<std::string> m_parameter;

				std::vector<uuid> m_statement;

		} symbol_function, *symbol_function_ptr;

		typedef class _symbol_identifier :
				public symbol {
		
			public:

				_symbol_identifier(
					__in _scope *parent,
					__in const std::string &key,
					__in const std::string &value
					);

				_symbol_identifier(
					__in _symbol_identifier &other
					);

				virtual ~_symbol_identifier(void);

				_symbol_identifier &operator=(
					__in _symbol_identifier &other
					);

				std::string &get_value(void);

				void set_value(
					__in const std::string &value
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::string m_value;

		} symbol_identifier, *symbol_identifier_ptr;

		typedef class _symbol_integer :
				public symbol {
		
			public:

				_symbol_integer(
					__in _scope *parent,
					__in const std::string &key,
					__in int value = 0
					);

				_symbol_integer(
					__in _symbol_integer &other
					);

				virtual ~_symbol_integer(void);

				_symbol_integer &operator=(
					__in _symbol_integer &other
					);

				int get_value(void);

				void set_value(
					__in int value
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				int m_value;

		} symbol_integer, *symbol_integer_ptr;

		typedef class _symbol_literal_string :
				public symbol {
		
			public:

				_symbol_literal_string(
					__in _scope *parent,
					__in const std::string &key,
					__in const std::string &value
					);

				_symbol_literal_string(
					__in _symbol_literal_string &other
					);

				virtual ~_symbol_literal_string(void);

				_symbol_literal_string &operator=(
					__in _symbol_literal_string &other
					);

				std::string get_value(void);

				void set_value(
					__in const std::string &value
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::string m_value;

		} symbol_literal_string, *symbol_literal_string_ptr;

		typedef class _symbol_null :
				public symbol {
		
			public:

				_symbol_null(
					__in _scope *parent,
					__in const std::string &key
					);

				_symbol_null(
					__in _symbol_null &other
					);

				virtual ~_symbol_null(void);

				_symbol_null &operator=(
					__in _symbol_null &other
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

		} symbol_null, *symbol_null_ptr;

		typedef class _symbol_structure :
				public symbol {
		
			public:

				_symbol_structure(
					__in _scope *parent,
					__in const std::string &key
					);

				_symbol_structure(
					__in _symbol_structure &other
					);

				virtual ~_symbol_structure(void);

				_symbol_structure &operator=(
					__in _symbol_structure &other
					);

				void add_value(
					__in const std::string &key,
					__in const uuid &value
					);

				void clear(void);

				uuid get_value(
					__in const std::string &key
					);

				bool has_value(
					__in const std::string &key
					);

				void remove_value(
					__in const std::string &key
					);

				void set_value(
					__in const std::string &key,
					__in const uuid &value
					);

				virtual size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::map<std::string, uuid>::iterator find_value(
					__in const std::string &key
					);

				std::map<std::string, uuid> m_value;

		} symbol_structure, *symbol_structure_ptr;

		typedef class _scope :
				public uuid_base {
		
			public:

				_scope(void);

				_scope(
					__in const _scope &other
					);

				virtual ~_scope(void);

				_scope &operator=(
					__in const _scope &other
					);

				uuid add_symbol_array(
					__in const std::string &key
					);

				uuid add_symbol_boolean(
					__in const std::string &key,
					__in_opt bool value = false
					);

				uuid add_symbol_float(
					__in const std::string &key,
					__in_opt double value = 0
					);

				uuid add_symbol_function(
					__in const std::string &key
					);

				uuid add_symbol_identifier(
					__in const std::string &key,
					__in const std::string &value
					);

				uuid add_symbol_integer(
					__in const std::string &key,
					__in int value = 0
					);

				uuid add_symbol_literal_string(
					__in const std::string &key,
					__in const std::string &value
					);

				uuid add_symbol_null(
					__in const std::string &key
					);

				uuid add_symbol_structure(
					__in const std::string &key
					);

				virtual void clear(void);

				symbol_ptr get_symbol(
					__in const uuid &id
					);

				symbol_ptr get_symbol(
					__in const std::string &key
					);

				bool has_symbol(
					__in const uuid &id
					);

				bool has_symbol(
					__in const std::string &key
					);

				void remove_symbol(
					__in const uuid &id
					);

				void remove_symbol(
					__in const std::string &key
					);

				virtual size_t size(void);

				static std::string symbol_as_string(
					__in symbol_ptr sym,
					__in_opt bool verbose = false
					);
				
				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::map<std::string, uuid>::iterator find_lookup_symbol(
					__in const std::string &key
					);

				std::map<uuid, symbol_ptr>::iterator find_symbol(
					__in const uuid &id
					);

				std::map<std::string, uuid> m_sym_lookup_map;

				std::map<uuid, symbol_ptr> m_sym_map; 

			private:

				std::recursive_mutex m_lock;

		} scope, *scope_ptr;

		typedef class _scope_factory {
			
			public:

				static _scope_factory *acquire(
					__in_opt generic_cb_t dest = NULL
					);

				void clear(void);

				void destroy(void);

				scope &generate(void);

				scope &get_scope(
					__in const uuid &id
					);

				bool has_scope(
					__in const uuid &id
					);

				void initialize(void);

				bool is_initialized(void);

				void remove_scope(
					__in const uuid &id
					);

				size_t size(void);

				std::string to_string(
					__in bool verbose = false
					);

			protected:

				friend void scope_factory_destroy(void);

				_scope_factory(
					__in_opt generic_cb_t dest = NULL
					);

				_scope_factory(
					__in const _node_factory &other
					);

				~_scope_factory(void);

				_scope_factory &operator=(
					__in const _scope_factory &other
					);

				std::map<uuid, scope>::iterator find_scope(
					__in const uuid &id
					);

				bool m_initialized;

				static _scope_factory *m_instance;

				std::map<uuid, scope> m_scope_map;

			private:

				std::recursive_mutex m_lock;

		} scope_factory, *scope_factory_ptr;
	}
}

#endif // LUNA_SCOPE_H_
