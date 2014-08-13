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
#include "luna_node_type.h"

namespace LUNA_NS {

	namespace LUNA_COMP_NS {
		
		_node_factory *_node_factory::m_instance = NULL;

		void 
		node_factory_destroy(void)
		{
			TRACE_ENTRY();

			if(node_factory::m_instance) {
				delete node_factory::m_instance;
				node_factory::m_instance = NULL;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_node::_node(
			__in const uuid &token_id,
			__in_opt size_t parent
			) :
				m_parent(parent),
				m_token_id(token_id)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_node::_node(
			__in const _node &other
			) :
				uuid_base(other),
				m_child(other.m_child),
				m_parent(other.m_parent),
				m_token_id(other.m_token_id)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_node::~_node(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_node &
		_node::operator=(
			__in const _node &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				uuid_base::operator=(other);
				m_child = other.m_child;
				m_parent = other.m_parent;
				m_token_id = other.m_token_id;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		void 
		_node::add_child(
			__in size_t offset
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_child.push_back(offset);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_node::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_child.clear();
			m_parent = PARENT_NODE_NONE;
			m_token_id.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_node::get_child(
			__in size_t position
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position >= m_child.size()) {
				THROW_LUNA_NODE_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_INVALID_NODE_POSITION,
					"pos/max: %lu/%lu", position, (m_child.size() - 1));
			}

			result = m_child.at(position);
			
			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_node::get_child_count(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_child.size();
			
			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_node::get_parent(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: %lu", m_parent);
			return m_parent;
		}

		uuid & 
		_node::get_token_id(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_token_id;
		}

		void 
		_node::insert_child(
			__in size_t position,
			__in size_t offset
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position > m_child.size()) {
				THROW_LUNA_NODE_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_INVALID_NODE_POSITION,
					"pos/max: %lu/%lu", position, m_child.size());
			}

			if(position < m_child.size()) {
				m_child.insert(m_child.begin() + position, offset);
			} else {
				m_child.push_back(offset);
			}
			
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		bool 
		_node::is_leaf(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_child.empty();
			
			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_node::is_root(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = (m_parent == PARENT_NODE_NONE);
			
			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_node::remove_child(
			__in size_t position
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position >= m_child.size()) {
				THROW_LUNA_NODE_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_INVALID_NODE_POSITION,
					"pos/max: %lu/%lu", position, (m_child.size() - 1));
			}

			m_child.erase(m_child.begin() + position);
			
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_node::set_child(
			__in size_t position,
			__in size_t offset
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(position >= m_child.size()) {
				THROW_LUNA_NODE_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_INVALID_NODE_POSITION,
					"pos/max: %lu/%lu", position, (m_child.size() - 1));
			}

			m_child.at(position) = offset;
			
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_node::set_parent(
			__in size_t offset
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_parent = offset;
			
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_node::set_token_id(
			__in const uuid &token_id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_token_id = token_id;
			
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::string 
		_node::to_string(
			__in bool verbose
			)
		{
			std::stringstream result;
			std::vector<size_t>::iterator child_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(verbose) {
				result << uuid_base::to_string(verbose) << " ";
			}

			result << "Token: " << m_token_id.to_string() << ", Parent: ";

			if(m_parent == PARENT_NODE_NONE) {
				result << NONE;
			} else {
				result << m_parent;
			}

			result << ", Children[" << m_child.size() << "]";

			if(verbose) {

				for(child_iter = m_child.begin(); child_iter != m_child.end(); ++child_iter) {
					
					if(child_iter == m_child.begin()) {
						result << " {" << *child_iter;
					} else {
						result << ", " << *child_iter;
					}

					if(child_iter == (m_child.end() - 1)) {
						result << "}";
					}
				}
			}

			if((m_parent == PARENT_NODE_NONE) || (m_child.empty())) {
				result << " (";

				if(m_parent == PARENT_NODE_NONE) {
					result << "ROOT";

					if(m_child.empty()) {
						result << ", ";
					}
				}

				if(m_child.empty()) {
					result << "LEAF";
				}

				result << ")";
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		_node_factory::_node_factory(
			__in_opt generic_cb_t dest
			)
		{
			TRACE_ENTRY();

			if(dest) {
				std::atexit(dest);
			} else {
				std::atexit(node_factory_destroy);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_node_factory::~_node_factory(void)
		{
			TRACE_ENTRY();

			destroy();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_node_factory *
		_node_factory::acquire(
			__in_opt generic_cb_t dest
			)
		{
			node_factory_ptr result;

			TRACE_ENTRY();

			if(!m_instance) {
			
				m_instance = new node_factory(dest);
				if(!m_instance) {
					THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_ALLOC_FAILED);
				}
			}

			result = m_instance;

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_node_factory::add_node(
			__in const uuid &id,
			__in const node &nd
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			if(m_node_map.find(id) != m_node_map.end()) {
				THROW_LUNA_NODE_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_ID_ALREADY_ADDED,
					"%s", uuid::id_as_string(id).c_str());
			}

			m_node_map.insert(std::pair<uuid, node>(id, nd));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_node_factory::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			m_node_map.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_node_factory::destroy(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_initialized) {
				clear();
				m_initialized = false;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::map<uuid, node>::iterator 
		_node_factory::find_node(
			__in const uuid &id
			)
		{
			std::map<uuid, node>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			result = m_node_map.find(id);
			if(result == m_node_map.end()) {
				THROW_LUNA_NODE_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_ID_NOT_FOUND,
					"%s", uuid::id_as_string(id).c_str());
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		node &
		_node_factory::generate(
			__in const uuid &token_id,
			__in_opt size_t parent
			)
		{
			node nd(token_id);

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			nd.set_parent(parent);
			add_node(nd.get_id(), nd);

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_node(nd.get_id())->second;
		}

		node &
		_node_factory::get_node(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return find_node(id)->second;
		}

		bool 
		_node_factory::has_node(
			__in const uuid &id
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			result = (m_node_map.find(id) != m_node_map.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_node_factory::initialize(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			destroy();
			m_initialized = true;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		bool 
		_node_factory::is_initialized(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_initialized;

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_node_factory::remove_node(
			__in const uuid &id
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			m_node_map.erase(find_node(id));

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		size_t 
		_node_factory::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			
			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			result = m_node_map.size();

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		std::string 
		_node_factory::to_string(
			__in bool verbose
			)
		{
			std::stringstream result;
			std::map<uuid, node>::iterator node_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_NODE_EXCEPTION(LUNA_NODE_EXCEPTION_UNINITIALIZED);
			}

			if(verbose) {
				result << "(" << (m_initialized ? "initialized" : "uninitialized") << ") ";
			}

			result << "Entries: " << m_node_map.size();
			
			for(node_iter = m_node_map.begin(); node_iter != m_node_map.end(); ++node_iter) {
				result << std::endl << node_iter->second.to_string(verbose);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}
	}
}
