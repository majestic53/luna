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

#ifndef LUNA_INTERPRETER_H_
#define LUNA_INTERPRETER_H_

#include <stack>

namespace LUNA_NS {

	using namespace LUNA_NS::LUNA_LANG_NS;

	namespace LUNA_RUNTIME_NS {
		
		typedef std::stack<uuid> scope_st_t, *scope_st_ptr_t;

		typedef class _interpreter :
				protected parser {
			
			public:

				_interpreter(void);

				_interpreter(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				_interpreter(
					__in const _interpreter &other
					);

				virtual ~_interpreter(void);

				_interpreter &operator=(
					__in const _interpreter &other
					);

				virtual void clear(void);

				void run(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				void step(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				void flush_scopes(void);

				scope_factory_ptr get_scope_factory(void);

				void pop_scope(void);

				scope_st_t m_scope_stack;

			private:

				std::recursive_mutex m_lock;

		} interpreter, *interpreter_ptr;
	}
}

#endif // LUNA_INTERPRETER_H_
