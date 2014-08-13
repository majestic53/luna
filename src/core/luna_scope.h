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

				void add_symbol(
					__in const std::string &key,
					__in const uuid &node_id
					);

				virtual void clear(void);

				uuid get_symbol(
					__in const std::string &key
					);

				bool has_symbol(
					__in const std::string &key
					);

				void remove_symbol(
					__in const std::string &key
					);

				void set_symbol(
					__in const std::string &key,
					__in const uuid &node_id
					);

				virtual size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::map<std::string, uuid>::iterator find_symbol(
					__in const std::string &key
					);

				std::map<std::string, uuid> m_sym_table;

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
