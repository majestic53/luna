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
#include "luna_uuid_type.h"

namespace LUNA_NS {

	namespace LUNA_COMP_NS {
		
		_uuid_factory *_uuid_factory::m_instance = NULL;

		bool 
		operator<(
			__in const uuid &left,
			__in const uuid &right
			)
		{
			bool result = false;

			TRACE_ENTRY();

			if(&left != &right) {
				result = (left.m_block < right.m_block);
			}

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		uuid_factory_destroy(void)
		{
			TRACE_ENTRY();

			if(uuid_factory::m_instance) {
				delete uuid_factory::m_instance;
				uuid_factory::m_instance = NULL;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid::_uuid(void) :
			m_block(0, 0)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid::_uuid(
			__in const uuid_blk_t &block
			) :
				m_block(block)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid::_uuid(
			__in const _uuid &other
			) :
				m_block(other.m_block)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid::~_uuid(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid &
		_uuid::operator=(
			__in const _uuid &other
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				m_block = other.m_block;
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		bool 
		_uuid::operator==(
			__in const _uuid &other
			)
		{
			bool result = true;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				result = (m_block == other.m_block);
			}

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid::operator!=(
			__in const _uuid &other
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = !(*this == other);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid::operator<(
			__in const _uuid &other
			)
		{
			bool result = true;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				result = (m_block < other.m_block);
			}

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid::operator>(
			__in const _uuid &other
			)
		{
			bool result = true;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				result = (m_block > other.m_block);
			}

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid::operator>=(
			__in const _uuid &other
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = !(*this < other);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid::operator<=(
			__in const _uuid &other
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = !(*this > other);

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		void 
		_uuid::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			m_block = uuid_blk_t(0, 0);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		uuid_blk_t &
		_uuid::get_id(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%p", &m_block);
			return m_block;
		}

		std::string 
		_uuid::id_as_string(
			__in const uuid_blk_t &block
			)
		{
			std::stringstream result;

			TRACE_ENTRY();

			result << "{" << VALUE_AS_HEX(uint32_t, block.first >> 0x20) << "-" 
				<< VALUE_AS_HEX(uint16_t, block.first >> 0x10) << "-"
				<< VALUE_AS_HEX(uint16_t, block.first) << "-"
				<< VALUE_AS_HEX(uint16_t, block.second >> 0x30) << "-"
				<< VALUE_AS_HEX(uint16_t, block.second >> 0x20) 
				<< VALUE_AS_HEX(uint32_t, block.second) << "}";

			TRACE_EXIT("Return Value: %s", result.str().c_str());
			return result.str();
		}

		std::string 
		_uuid::id_as_string(
			__in const _uuid &id
			)
		{
			return id_as_string(id.m_block);
		}

		std::string 
		_uuid::to_string(
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			UNREF_PARAM(verbose);
			return id_as_string(m_block);
		}

		_uuid_factory::_uuid_factory(
			__in_opt generic_cb_t dest
			) :
				m_initialized(false),
				m_seed(0)
		{
			TRACE_ENTRY();

			if(dest) {
				std::atexit(dest);
			} else {
				std::atexit(uuid_factory_destroy);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid_factory::~_uuid_factory(void)
		{
			TRACE_ENTRY();

			destroy();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid_factory *
		_uuid_factory::acquire(
			__in_opt generic_cb_t dest
			)
		{
			uuid_factory_ptr result = NULL;

			TRACE_ENTRY();

			if(!m_instance) {
				
				m_instance = new uuid_factory(dest);
				if(!m_instance) {
					THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_ALLOC_FAILED);
				}
			}
			
			result = m_instance;

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_uuid_factory::clear(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			m_generator.seed(m_seed);
			m_id_map.clear();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		void 
		_uuid_factory::destroy(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(m_initialized) {
				clear();
				m_initialized = false;
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		std::map<uuid, uuid_config_t>::iterator 
		_uuid_factory::find_id(
			__in const uuid &id
			)
		{
			std::map<uuid, uuid_config_t>::iterator result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			result = m_id_map.find(id);
			if(result == m_id_map.end()) {
				THROW_LUNA_UUID_EXCEPTION_MESSAGE(LUNA_UUID_EXCEPTION_ID_NOT_FOUND, 
					"%s", uuid::id_as_string(id).c_str());
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

size_t 
		_uuid_factory::generate(
			__inout uuid &id,
			__in_opt bool unique
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			do {
				id = uuid(generate_block());

				if(!unique) {
					break;
				}
			} while(is_registered(id));

			result = register_id(id);

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		uuid 
		_uuid_factory::generate(
			__in_opt bool unique
			)
		{
			uuid id;

			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			generate(id, unique);

			TRACE_EXIT("Return Value: %s", id.to_string());
			return id;
		}

		uuid_blk_t 
		_uuid_factory::generate_block(void)
		{
			uuid_blk_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = uuid_blk_t(
				((uint64_t) m_distribution(m_generator) << 0x20) | m_distribution(m_generator),
				((uint64_t) m_distribution(m_generator) << 0x20) | m_distribution(m_generator));

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result;
		}

		size_t 
		_uuid_factory::get_reference_count(
			__in const uuid &id
			)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			result = find_id(id)->second.first;

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		void 
		_uuid_factory::initialize(
			__in uint32_t seed
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			destroy();
			m_seed = seed;
			m_generator.seed(seed);
			m_initialized = true;

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		bool 
		_uuid_factory::is_initialized(void)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			result = m_initialized;

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid_factory::is_registered(
			__in const uuid &id
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			result = (m_id_map.find(id) != m_id_map.end());

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		bool 
		_uuid_factory::is_unique(
			__in const uuid &id
			)
		{
			bool result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			result = find_id(id)->second.second;

			TRACE_EXIT("Return Value: 0x%x", result);
			return result;
		}

		size_t 
		_uuid_factory::register_id(
			__in const uuid &id,
			__in_opt bool unique
			)
		{
			size_t result = INIT_REF;
			std::map<uuid, uuid_config_t>::iterator id_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			id_iter = m_id_map.find(id);
			if(id_iter == m_id_map.end()) {
				m_id_map.insert(std::pair<uuid, uuid_config_t>(id, uuid_config_t(INIT_REF, unique)));
			} else if(id_iter->second.second) {
				THROW_LUNA_UUID_EXCEPTION_MESSAGE(LUNA_UUID_EXCEPTION_UNIQUE_ID_REFERENCED,
					"%s", uuid::id_as_string(id).c_str());
			} else {
				result = ++id_iter->second.first;
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		size_t 
		_uuid_factory::size(void)
		{
			size_t result;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			result = m_id_map.size();

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		std::string 
		_uuid_factory::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<uuid, uuid_config_t>::iterator id_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			if(verbose) {
				result << "(" << (m_initialized ? "initialized" : "uninitialized") << ") ";
			}

			result << "Entries: " << m_id_map.size() << ", Seed: 0x" << VALUE_AS_HEX(uint32_t, m_seed);
			
			for(id_iter = m_id_map.begin(); id_iter != m_id_map.end(); ++id_iter) {
				result << std::endl << "(" << (id_iter->second.second ? std::string() : "non-") 
					<< "unique, ref. " << id_iter->second.first << ") " 
					<< uuid::id_as_string(id_iter->first);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
			return result.str();
		}

		size_t 
		_uuid_factory::unregister_id(
			__in const uuid &id
			)
		{
			size_t result = 0;
			std::map<uuid, uuid_config_t>::iterator id_iter;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(!m_initialized) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_UNINITIALIZED);
			}

			id_iter = m_id_map.find(id);
			if(id_iter == m_id_map.end()) {
				THROW_LUNA_UUID_EXCEPTION_MESSAGE(LUNA_UUID_EXCEPTION_ID_NOT_FOUND,
					"%s", uuid::id_as_string(id).c_str());
			} else if(id_iter->second.first == INIT_REF) {
				m_id_map.erase(id_iter);
			} else {
				result = --id_iter->second.first;
			}

			TRACE_EXIT("Return Value: %lu", result);
			return result;
		}

		_uuid_base::_uuid_base(void)
		{
			TRACE_ENTRY();

			generate_id();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid_base::_uuid_base(
			__in const _uuid_base &other
			)
		{
			uuid_factory_ptr uuid_inst;

			TRACE_ENTRY();

			uuid_inst = acquire_id_factory();
			if(!uuid_inst) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_ACQUIRY_FAILED);
			}
			
			m_id = other.m_id;
			uuid_inst->register_id(m_id);

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid_base::~_uuid_base(void)
		{
			uuid_factory_ptr uuid_inst;

			TRACE_ENTRY();

			uuid_inst = acquire_id_factory();
			if(uuid_inst) {
				uuid_inst->unregister_id(m_id);
			}

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		_uuid_base &
		_uuid_base::operator=(
			__in const _uuid_base &other
			)
		{
			uuid_factory_ptr uuid_inst;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(this != &other) {
				
				uuid_inst = acquire_id_factory();
				if(!uuid_inst) {
					THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_ACQUIRY_FAILED);
				}
				
				uuid_inst->unregister_id(m_id);
				m_id = other.m_id;
				uuid_inst->register_id(m_id);
			}

			TRACE_EXIT("Return Value: 0x%p", this);
			return *this;
		}

		uuid_factory_ptr 
		_uuid_base::acquire_id_factory(void)
		{
			luna_ptr inst;
			uuid_factory_ptr result = NULL;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			if(luna::is_globally_initialized()) {

				inst = luna::acquire();
				if(!inst) {
					THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_LIBARY_ACQUIRY_FAILED);
				}

				if(inst->is_initialized()) {

					result = inst->acquire_uuid_factory();
					if(!result) {
						THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_ACQUIRY_FAILED);
					}
				}
			}

			TRACE_EXIT("Return Value: 0x%p", result);
			return result;
		}

		void 
		_uuid_base::generate_id(void)
		{
			uuid_factory_ptr uuid_inst;

			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);

			uuid_inst = acquire_id_factory();
			if(!uuid_inst) {
				THROW_LUNA_UUID_EXCEPTION(LUNA_UUID_EXCEPTION_ACQUIRY_FAILED);
			}

			m_id = uuid_inst->generate();

			TRACE_EXIT("Return Value: 0x%x", NULL);
		}

		uuid &
		_uuid_base::get_id(void)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%p", &m_id);
			return m_id;
		}

		std::string 
		_uuid_base::to_string(
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY();
			SERIALIZE_CALL_RECUR(m_lock);
			TRACE_EXIT("Return Value: 0x%x", NULL);
			return m_id.to_string(verbose);
		}
	}
}
