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

#ifndef LUNA_NODE_H_
#define LUNA_NODE_H_

#include <vector>

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		#define PARENT_NODE_NONE ((size_t) INVALID)

		typedef class _node :
				public uuid_base {
		
			public:

				_node(
					__in const uuid &token_id,
					__in_opt size_t parent = PARENT_NODE_NONE
					);

				_node(
					__in const _node &other
					);

				virtual ~_node(void);

				_node &operator=(
					__in const _node &other
					);

				void add_child(
					__in size_t offset
					);

				virtual void clear(void);

				size_t get_child(
					__in size_t position
					);

				size_t get_child_count(void);

				size_t get_parent(void);

				uuid &get_token_id(void);

				void insert_child(
					__in size_t position,
					__in size_t offset
					);

				bool is_leaf(void);

				bool is_root(void);

				void remove_child(
					__in size_t position
					);

				void set_child(
					__in size_t position,
					__in size_t offset
					);

				void set_parent(
					__in size_t offset
					);

				void set_token_id(
					__in const uuid &token_id
					);

				virtual std::string to_string(
					__in bool verbose = false
					);

			protected:

				std::vector<size_t> m_child;

				size_t m_parent;

				uuid m_token_id;

			private:

				std::recursive_mutex m_lock;

		} node, *node_ptr;

		void node_factory_destroy(void);

		typedef class _node_factory {
		
			public:

				static _node_factory *acquire(
					__in_opt generic_cb_t dest = NULL
					);

				void add_node(
					__in const uuid &id,
					__in const node &nd
					);

				void clear(void);

				void destroy(void);

				node &generate(
					__in const uuid &token_id,
					__in_opt size_t parent = PARENT_NODE_NONE
					);

				node &get_node(
					__in const uuid &id
					);

				bool has_node(
					__in const uuid &id
					);

				void initialize(void);

				bool is_initialized(void);

				void remove_node(
					__in const uuid &id
					);

				size_t size(void);

				std::string to_string(
					__in bool verbose = false
					);

			protected:

				friend void node_factory_destroy(void);

				_node_factory(
					__in_opt generic_cb_t dest = NULL
					);

				_node_factory(
					__in const _node_factory &other
					);

				~_node_factory(void);

				_node_factory &operator=(
					__in const _node_factory &other
					);

				std::map<uuid, node>::iterator find_node(
					__in const uuid &id
					);

				bool m_initialized;

				static _node_factory *m_instance;

				std::map<uuid, node> m_node_map;

			private:

				std::recursive_mutex m_lock;

		} node_factory, *node_factory_ptr;
	}
}

#endif // LUNA_NODE_H_
