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

#ifndef LUNA_UUID_H_
#define LUNA_UUID_H_

#include <map>
#include <random>

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		typedef std::pair<uint64_t, uint64_t> uuid_blk_t, *uuid_blk_ptr_t;
		typedef std::pair<size_t, bool> uuid_config_t, *uuid_config_ptr_t;

		typedef class _uuid {
		
			public:

				_uuid(void);

				_uuid(
					__in const uuid_blk_t &block
					);

				_uuid(
					__in const _uuid &other
					);

				virtual ~_uuid(void);

				_uuid &operator=(
					__in const _uuid &other
					);

				bool operator==(
					__in const _uuid &other
					);

				bool operator!=(
					__in const _uuid &other
					);

				bool operator<(
					__in const _uuid &other
					);

				bool operator>(
					__in const _uuid &other
					);

				bool operator>=(
					__in const _uuid &other
					);

				bool operator<=(
					__in const _uuid &other
					);

				virtual void clear(void);

				uuid_blk_t &get_id(void);

				static std::string id_as_string(
					__in const _uuid &id
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				friend class _uuid_factory;

				friend bool operator<(
					__in const _uuid &left,
					__in const _uuid &right
					);

				static std::string id_as_string(
					__in const uuid_blk_t &block
					);

				uuid_blk_t m_block;

			private:

				std::recursive_mutex m_lock;

		} uuid, *uuid_ptr;

		bool operator<(
			__in const uuid &left,
			__in const uuid &right
			);

		void uuid_factory_destroy(void);

		typedef class _uuid_factory {
		
			public:

				static _uuid_factory *acquire(
					__in_opt generic_cb_t dest = NULL
					);

				void clear(void);

				void destroy(void);

				size_t generate(
					__inout uuid &id,
					__in_opt bool unique = false
					);

				uuid generate(
					__in_opt bool unique = false
					);

				size_t get_reference_count(
					__in const uuid &id
					);

				void initialize(
					__in uint32_t seed
					);

				bool is_initialized(void);

				bool is_registered(
					__in const uuid &id
					);

				bool is_unique(
					__in const uuid &id
					);

				size_t register_id(
					__in const uuid &id,
					__in_opt bool unique = false
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				size_t unregister_id(
					__in const uuid &id
					);

			protected:

				friend void uuid_factory_destroy(void);

				_uuid_factory(
					__in_opt generic_cb_t dest = NULL
					);

				_uuid_factory(
					__in const _uuid_factory &other
					);

				~_uuid_factory(void);

				_uuid_factory &operator=(
					__in const _uuid_factory &other
					);

				std::map<uuid, uuid_config_t>::iterator find_id(
					__in const uuid &id
					);

				uuid_blk_t generate_block(void);

				std::uniform_int_distribution<uint32_t> m_distribution;

				std::mt19937 m_generator;

				std::map<uuid, uuid_config_t> m_id_map;

				bool m_initialized;

				static _uuid_factory *m_instance;

				uint32_t m_seed;

			private:

				std::recursive_mutex m_lock;

		} uuid_factory, *uuid_factory_ptr;

		typedef class _uuid_base {
		
			public:

				_uuid_base(void);

				_uuid_base(
					__in const _uuid_base &other
					);

				virtual ~_uuid_base(void);

				_uuid_base &operator=(
					__in const _uuid_base &other
					);

				uuid &get_id(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				uuid_factory_ptr acquire_id_factory(void);

				void generate_id(void);

				uuid m_id;

			private:

				std::recursive_mutex m_lock;

		} uuid_base, *uuid_base_ptr;
	}
}

#endif // LUNA_UUID_H_
