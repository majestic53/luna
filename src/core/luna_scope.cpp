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
		
		static const std::string SCOPE_SYMBOL_STR[] = {
			"ARRAY", "BOOLEAN", "FLOAT", "FUNCTION", "IDENTIFIER", "INTEGER",
			"NULL", "STRUCTURE",
			};

		#define SCOPE_SYMBOL_STRING(_TYPE_)\
			((_TYPE_) > SCOPE_SYMBOL_MAX ? UNKNOWN : SCOPE_SYMBOL_STR[_TYPE_].c_str())

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

		_symbol::_symbol(
			__in _scope *parent,
			__in const std::string &key,
			__in scope_sym_t type
			) :
				m_parent(parent)
		{
			TRACE_ENTRY();

			symbol::set_key(key);
			symbol::set_type(type);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol::_symbol(
			__in const _symbol &other
			) :
				uuid_base(other),
				m_key(other.m_key),
				m_parent(other.m_parent),
				m_type(other.m_type)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol::~_symbol(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol &
		_symbol::operator=(
			__in const _symbol &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				uuid_base::operator=(other);
				m_key = other.m_key;
				m_parent = other.m_parent;
				m_type = other.m_type;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		std::string &
		_symbol::get_key(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_key;
		}

		scope_sym_t 
		_symbol::get_type(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_type;
		}

		void 
		_symbol::set_key(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(key.empty()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_KEY,
					"\'%s\' (%lu)", key.c_str(), key.size());
			}

			m_key = key;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol::set_type(
			__in scope_sym_t type
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(type > SCOPE_SYMBOL_MAX) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_TYPE,
					"%lu", type);
			}

			m_type = type;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << uuid_base::to_string() << " ";
			}	

			result << "[" << SCOPE_SYMBOL_STRING(m_type) << "] \'" << m_key << "\'";

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_array::_symbol_array(
			__in _scope *parent,
			__in const std::string &key	
			) :
				symbol(parent, key, SCOPE_SYMBOL_ARRAY)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_array::_symbol_array(
			__in _symbol_array &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_array::~_symbol_array(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_array &
		_symbol_array::operator=(
			__in _symbol_array &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_symbol_array::add_value(
			__in const uuid &value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value.push_back(value);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol_array::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		uuid 
		_symbol_array::get_value(
			__in size_t index
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(index >= m_value.size()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
					"ind/max: %lu/%lu", index, m_value.size() - 1);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_value.at(index);
		}

		void 
		_symbol_array::insert_value(
			__in const uuid &value,
			__in size_t index
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(index > m_value.size()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
					"ind/max: %lu/%lu", index, m_value.size());
			} else if(index < m_value.size()) {
				m_value.insert(m_value.begin() + index, value);
			} else {
				m_value.push_back(value);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol_array::remove_value(
			__in size_t index
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(index >= m_value.size()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
					"ind/max: %lu/%lu", index, m_value.size() - 1);
			}

			m_value.erase(m_value.begin() + index);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol_array::set_value(
			__in const uuid &value,
			__in size_t index
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(index >= m_value.size()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
					"ind/max: %lu/%lu", index, m_value.size() - 1);
			}

			m_value.at(index) = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_symbol_array::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_value.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_symbol_array::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::vector<uuid>::iterator sym_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Entries: " << m_value.size();

			for(sym_iter = m_value.begin(); sym_iter != m_value.end(); ++sym_iter) {
				result << std::endl << "--- " << (m_parent ? scope::symbol_as_string(m_parent->get_symbol(*sym_iter))
					: sym_iter->to_string());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_boolean::_symbol_boolean(
			__in _scope *parent,
			__in const std::string &key,
			__in_opt bool value
			) :
				symbol(parent, key, SCOPE_SYMBOL_BOOLEAN),
				m_value(value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_boolean::_symbol_boolean(
			__in _symbol_boolean &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_boolean::~_symbol_boolean(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_boolean &
		_symbol_boolean::operator=(
			__in _symbol_boolean &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		bool 
		_symbol_boolean::get_value(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_value;
		}

		void 
		_symbol_boolean::set_value(
			__in bool value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_boolean::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Value: " 
				<< (m_value ? "TRUE" : "FALSE");

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_float::_symbol_float(
			__in _scope *parent,
			__in const std::string &key,
			__in_opt double value
			) :
				symbol(parent, key, SCOPE_SYMBOL_FLOAT),
				m_value(value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_float::_symbol_float(
			__in _symbol_float &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_float::~_symbol_float(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_float &
		_symbol_float::operator=(
			__in _symbol_float &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		double 
		_symbol_float::get_value(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_value;
		}

		void 
		_symbol_float::set_value(
			__in double value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_float::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Value: " << m_value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_function::_symbol_function(
			__in _scope *parent,
			__in const std::string &key
			) :
				symbol(parent, key, SCOPE_SYMBOL_FUNCTION)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_function::_symbol_function(
			__in _symbol_function &other
			) :
				symbol(other),
				m_parameter(other.m_parameter),
				m_statement(other.m_statement)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_function::~_symbol_function(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_function &
		_symbol_function::operator=(
			__in _symbol_function &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_parameter = other.m_parameter;
				m_statement = other.m_statement;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_symbol_function::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_parameter.clear();
			m_statement.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_function::get_paramter(
			__in size_t index
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(index >= m_parameter.size()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
					"ind/max: %lu/%lu", index, m_parameter.size() - 1);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_parameter.at(index);
		}

		size_t 
		_symbol_function::get_paramter_count(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_parameter.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		uuid 
		_symbol_function::get_statement(
			__in size_t index
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(index >= m_statement.size()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_INDEX,
					"ind/max: %lu/%lu", index, m_statement.size() - 1);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_statement.at(index);
		}

		size_t 
		_symbol_function::get_statement_count(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_statement.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		void 
		_symbol_function::set_parameters(
			__in const std::vector<std::string> &parameters
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_parameter = parameters;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol_function::set_statements(
			__in const std::vector<uuid> &statements
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_statement = statements;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_function::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::vector<uuid>::iterator stmt_iter;
			std::vector<std::string>::iterator param_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Parameters(" << m_parameter.size() << "): {";

			for(param_iter = m_parameter.begin(); param_iter != m_parameter.end(); ++param_iter) {
				
				if(param_iter != m_parameter.begin()) {
					result << ", ";
				}

				result << *param_iter;
			}

			result << "}, Statements: " << m_statement.size();

			for(stmt_iter = m_statement.begin(); stmt_iter != m_statement.end(); ++stmt_iter) {
				result << "--- " << stmt_iter->to_string();
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_identifier::_symbol_identifier(
			__in _scope *parent,
			__in const std::string &key,
			__in_opt const std::string &value
			) :
				symbol(parent, key, SCOPE_SYMBOL_IDENTIFIER),
				m_value(value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_identifier::_symbol_identifier(
			__in _symbol_identifier &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_identifier::~_symbol_identifier(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_identifier &
		_symbol_identifier::operator=(
			__in _symbol_identifier &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		std::string &
		_symbol_identifier::get_value(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_value;
		}

		void 
		_symbol_identifier::set_value(
			__in const std::string &value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_identifier::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Value: \'" << m_value << "\'";

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_integer::_symbol_integer(
			__in _scope *parent,
			__in const std::string &key,
			__in int value
			) :
				symbol(parent, key, SCOPE_SYMBOL_INTEGER),
				m_value(value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_integer::_symbol_integer(
			__in _symbol_integer &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_integer::~_symbol_integer(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_integer &
		_symbol_integer::operator=(
			__in _symbol_integer &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		int 
		_symbol_integer::get_value(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_value;
		}

		void 
		_symbol_integer::set_value(
			__in int value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_integer::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Value: " << m_value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_literal_string::_symbol_literal_string(
			__in _scope *parent,
			__in const std::string &key,
			__in_opt const std::string &value
			) :
				symbol(parent, key, SCOPE_SYMBOL_LITERAL_STRING),
				m_value(value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_literal_string::_symbol_literal_string(
			__in _symbol_literal_string &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_literal_string::~_symbol_literal_string(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_literal_string &
		_symbol_literal_string::operator=(
			__in _symbol_literal_string &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		std::string 
		_symbol_literal_string::get_value(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_value;
		}

		void 
		_symbol_literal_string::set_value(
			__in const std::string &value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_symbol_literal_string::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Value: \"" << m_value << "\"";

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_symbol_null::_symbol_null(
			__in _scope *parent,
			__in const std::string &key
			) :
				symbol(parent, key, SCOPE_SYMBOL_NULL)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_null::_symbol_null(
			__in _symbol_null &other
			) :
				symbol(other)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_null::~_symbol_null(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_null &
		_symbol_null::operator=(
			__in _symbol_null &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		std::string 
		_symbol_null::to_string(
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return symbol::to_string(verbose);
		}

		_symbol_structure::_symbol_structure(
			__in _scope *parent,
			__in const std::string &key
			) :
				symbol(parent, key, SCOPE_SYMBOL_STRUCTURE)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_structure::_symbol_structure(
			__in _symbol_structure &other
			) :
				symbol(other),
				m_value(other.m_value)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_structure::~_symbol_structure(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_symbol_structure &
		_symbol_structure::operator=(
			__in _symbol_structure &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				symbol::operator=(other);
				m_value = other.m_value;
			}

			TRACE_EXIT("Return Value: 0x%x", this);
			return *this;
		}

		void 
		_symbol_structure::add_value(
			__in const std::string &key,
			__in const uuid &value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_value.find(key) != m_value.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_KEY_ALREADY_EXISTS,
					"%s", key.c_str());
			}

			m_value.insert(std::pair<std::string, uuid>(key, value));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol_structure::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}
		
		std::map<std::string, uuid>::iterator 
		_symbol_structure::find_value(
			__in const std::string &key
			)
		{
			std::map<std::string, uuid>::iterator key_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			key_iter = m_value.find(key);
			if(key_iter == m_value.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_KEY_NOT_FOUND,
					"%s", key.c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return key_iter;
		}

		uuid 
		_symbol_structure::get_value(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_value(key)->second;
		}

		bool 
		_symbol_structure::has_value(
			__in const std::string &key
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_value.find(key) != m_value.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_symbol_structure::remove_value(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_value.erase(find_value(key));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_symbol_structure::set_value(
			__in const std::string &key,
			__in const uuid &value
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			find_value(key)->second = value;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_symbol_structure::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_value.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_symbol_structure::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<std::string, uuid>::iterator sym_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result << symbol::to_string(verbose) << ", Entries: " << m_value.size();

			for(sym_iter = m_value.begin(); sym_iter != m_value.end(); ++sym_iter) {
				result << std::endl << "--- " << (m_parent ? scope::symbol_as_string(m_parent->get_symbol(sym_iter->second))
					: sym_iter->second.to_string());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
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
				m_sym_lookup_map(other.m_sym_lookup_map),
				m_sym_map(other.m_sym_map)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_scope::~_scope(void)
		{
			TRACE_ENTRY();

			scope::clear();

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
				m_sym_lookup_map = other.m_sym_lookup_map;
				m_sym_map = other.m_sym_map;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		uuid 
		_scope::add_symbol_array(
			__in const std::string &key
			)
		{
			uuid result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_symbol(allocate_symbol(key, SCOPE_SYMBOL_ARRAY));

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_boolean(
			__in const std::string &key,
			__in_opt bool value
			)
		{
			uuid result;
			symbol_boolean_ptr sym;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			sym = (symbol_boolean_ptr) allocate_symbol(key, SCOPE_SYMBOL_BOOLEAN);
			if(!sym) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL);
			}
			
			sym->set_value(value);
			result = add_symbol(sym);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_float(
			__in const std::string &key,
			__in_opt double value
			)
		{
			uuid result;
			symbol_float_ptr sym;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			sym = (symbol_float_ptr) allocate_symbol(key, SCOPE_SYMBOL_FLOAT);
			if(!sym) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL);
			}
			
			sym->set_value(value);
			result = add_symbol(sym);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_function(
			__in const std::string &key
			)
		{
			uuid result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_symbol(allocate_symbol(key, SCOPE_SYMBOL_FUNCTION));

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_identifier(
			__in const std::string &key,
			__in const std::string &value
			)
		{
			uuid result;
			symbol_identifier_ptr sym;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			sym = (symbol_identifier_ptr) allocate_symbol(key, SCOPE_SYMBOL_IDENTIFIER);
			if(!sym) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL);
			}
			
			sym->set_value(value);
			result = add_symbol(sym);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_integer(
			__in const std::string &key,
			__in int value
			)
		{
			uuid result;
			symbol_integer_ptr sym;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			sym = (symbol_integer_ptr) allocate_symbol(key, SCOPE_SYMBOL_INTEGER);
			if(!sym) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL);
			}
			
			sym->set_value(value);
			result = add_symbol(sym);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_literal_string(
			__in const std::string &key,
			__in const std::string &value
			)
		{
			uuid result;
			symbol_literal_string_ptr sym;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			sym = (symbol_literal_string_ptr) allocate_symbol(key, SCOPE_SYMBOL_LITERAL_STRING);
			if(!sym) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL);
			}
			
			sym->set_value(value);
			result = add_symbol(sym);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_null(
			__in const std::string &key
			)
		{
			uuid result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_symbol(allocate_symbol(key, SCOPE_SYMBOL_NULL));

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol_structure(
			__in const std::string &key
			)
		{
			uuid result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = add_symbol(allocate_symbol(key, SCOPE_SYMBOL_STRUCTURE));

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		uuid 
		_scope::add_symbol(
			__in symbol_ptr sym
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!sym) {
				THROW_LUNA_SCOPE_EXCEPTION(LUNA_SCOPE_EXCEPTION_INVALID_SYMBOL);
			}

			if((m_sym_lookup_map.find(sym->get_key()) != m_sym_lookup_map.end())
					|| (m_sym_map.find(sym->get_id()) != m_sym_map.end())) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_KEY_ALREADY_EXISTS,
					"%s", sym->get_key().c_str());
			}

			m_sym_lookup_map.insert(std::pair<std::string, uuid>(sym->get_key(), sym->get_id()));
			m_sym_map.insert(std::pair<uuid, symbol_ptr>(sym->get_id(), sym));
			
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return sym->get_id();
		}

		symbol_ptr 
		_scope::allocate_symbol(
			__in const std::string &key,
			__in scope_sym_t type
			)
		{
			symbol_ptr result = NULL;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			switch(type) {
				case SCOPE_SYMBOL_ARRAY:
					result = new symbol_array(this, key);
					break;
				case SCOPE_SYMBOL_BOOLEAN:
					result = new symbol_boolean(this, key);
					break;
				case SCOPE_SYMBOL_FLOAT:
					result = new symbol_float(this, key);
					break;
				case SCOPE_SYMBOL_FUNCTION:
					result = new symbol_function(this, key);
					break;
				case SCOPE_SYMBOL_IDENTIFIER:
					result = new symbol_identifier(this, key);
					break;
				case SCOPE_SYMBOL_INTEGER:
					result = new symbol_integer(this, key);
					break;
				case SCOPE_SYMBOL_LITERAL_STRING:
					result = new symbol_literal_string(this, key);
					break;
				case SCOPE_SYMBOL_NULL:
					result = new symbol_null(this, key);
					break;
				case SCOPE_SYMBOL_STRUCTURE:
					result = new symbol_structure(this, key);
					break;
				default:
					THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_INVALID_TYPE,
						"%lu", type);
			}

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_scope::clear(void)
		{
			std::map<uuid, symbol_ptr>::iterator sym_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			for(sym_iter = m_sym_map.begin(); sym_iter != m_sym_map.end();
					++sym_iter) {
				
				if(!sym_iter->second) {
					continue;
				}

				delete sym_iter->second;
				sym_iter->second = NULL;
			}

			m_sym_map.clear();
			m_sym_lookup_map.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::map<std::string, uuid>::iterator 
		_scope::find_lookup_symbol(
			__in const std::string &key
			)
		{
			std::map<std::string, uuid>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_sym_lookup_map.find(key);
			if(result == m_sym_lookup_map.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_KEY_NOT_FOUND, 
					"%s", key.c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		std::map<uuid, symbol_ptr>::iterator 
		_scope::find_symbol(
			__in const uuid &id
			)
		{
			std::map<uuid, symbol_ptr>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_sym_map.find(id);
			if(result == m_sym_map.end()) {
				THROW_LUNA_SCOPE_EXCEPTION_MESSAGE(LUNA_SCOPE_EXCEPTION_ID_NOT_FOUND, 
					"%s", uuid::id_as_string(id).c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		symbol_ptr 
		_scope::get_symbol(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_symbol(id)->second;
		}

		symbol_ptr 
		_scope::get_symbol(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return get_symbol(find_lookup_symbol(key)->second);
		}

		bool 
		_scope::has_symbol(
			__in const uuid &id
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_sym_map.find(id) != m_sym_map.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
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

			result = (m_sym_lookup_map.find(key) != m_sym_lookup_map.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_scope::remove_symbol(
			__in const uuid &id
			)
		{
			std::map<uuid, symbol_ptr>::iterator sym_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			sym_iter = find_symbol(id);
			if(sym_iter->second) {
				delete sym_iter->second;
				sym_iter->second = NULL;
			}

			m_sym_map.erase(sym_iter);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_scope::remove_symbol(
			__in const std::string &key
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_sym_lookup_map.erase(find_lookup_symbol(key));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_scope::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_sym_lookup_map.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_scope::symbol_as_string(
			__in symbol_ptr sym,
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY();
			
			if(sym) {

				switch(sym->get_type()) {
					case SCOPE_SYMBOL_ARRAY:
						result << ((symbol_array_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_BOOLEAN:
						result << ((symbol_boolean_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_FLOAT:
						result << ((symbol_float_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_FUNCTION:
						result << ((symbol_function_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_IDENTIFIER:
						result << ((symbol_identifier_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_INTEGER:
						result << ((symbol_integer_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_LITERAL_STRING:
						result << ((symbol_literal_string_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_NULL:
						result << ((symbol_null_ptr) sym)->to_string(verbose);
						break;
					case SCOPE_SYMBOL_STRUCTURE:
						result << ((symbol_structure_ptr) sym)->to_string(verbose);
						break;
					default:
						result << UNKNOWN;
						break;
				}
			} else {
				result << "(null)";
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		std::string 
		_scope::to_string(
			__in_opt bool verbose
			)
		{
			size_t count = 0;
			std::stringstream result;
			std::map<std::string, uuid>::iterator sym_lookup_iter;
			std::map<uuid, symbol_ptr>::iterator sym_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << uuid_base::to_string(verbose) << " ";
			}

			result << "Entries: " << m_sym_lookup_map.size();

			if(verbose) {
				
				for(sym_lookup_iter = m_sym_lookup_map.begin(), sym_iter = m_sym_map.begin(); 
						sym_lookup_iter != m_sym_lookup_map.end(), sym_iter != m_sym_map.end(); 
						++sym_lookup_iter, ++sym_iter, ++count) {
					result << "--- [" << count << "/" << m_sym_map.size() << "] \'" << 
						sym_lookup_iter->first << "\': " << symbol_as_string(sym_iter->second);
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
