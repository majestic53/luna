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

#ifndef LUNA_H_
#define LUNA_H_

#ifndef LUNA_NS
#define LUNA_NS luna_ns
#endif // LUNA_NS

#include "luna_defines.h"
#include "luna_exception.h"
#include "luna_trace.h"
#include "luna_version.h"

#ifndef LUNA_LANG_NS
#define LUNA_LANG_NS lang_ns
#endif // LUNA_LANG_NS

#include "luna_language.h"

#ifndef LUNA_COMP_NS
#define LUNA_COMP_NS comp_ns
#endif // LUNA_COMP_NS

#include "luna_uuid.h"
#include "luna_node.h"
#include "luna_scope.h"
#include "luna_token.h"

#include "luna_lexer.h"
#include "luna_parser.h"

#ifndef LUNA_RUNTIME_NS
#define LUNA_RUNTIME_NS runtime_ns
#endif // LUNA_RUNTIME_NS

#include "luna_interpreter.h"

using namespace LUNA_NS;
using namespace LUNA_NS::LUNA_LANG_NS;
using namespace LUNA_NS::LUNA_COMP_NS;
using namespace LUNA_NS::LUNA_RUNTIME_NS;

namespace LUNA_NS {

	void luna_destroy(void);

	typedef class _luna {
	
		public:

			static _luna *acquire(
				__in_opt generic_cb_t dest = NULL
				);

			node_factory_ptr acquire_node_factory(void);

			scope_factory_ptr acquire_scope_factory(void);

			token_factory_ptr acquire_token_factory(void);

			uuid_factory_ptr acquire_uuid_factory(void);

			void destroy(void);

			void initialize(void);

			static bool is_globally_initialized(void);

			bool is_initialized(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			static std::string version(
				__in bool verbose,
				__in_opt bool show_title = false,
				__in_opt bool show_copyright = false
				);

		protected:

			friend void luna_destroy(void);

			_luna(
				__in_opt generic_cb_t dest = NULL
				);

			_luna(
				__in const _luna &other
				);

			~_luna(void);

			_luna &operator=(
				__in const _luna &other
				);

			static bool m_glob_initalized;

			bool m_initialized;

			static _luna *m_instance;

			node_factory_ptr m_node_factory;

			scope_factory_ptr m_scope_factory;

			token_factory_ptr m_token_factory;

			uuid_factory_ptr m_uuid_factory;

		private:

			std::recursive_mutex m_lock;

	} luna, *luna_ptr;
}

#endif // LUNA_H_
