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

#ifndef LUNA_TOKEN_H_
#define LUNA_TOKEN_H_

namespace LUNA_NS {

	using namespace LUNA_LANG_NS;

	namespace LUNA_COMP_NS {

		#define TOKEN_SUBTYPE_INVALID ((uint32_t) INVALID)

		typedef class _token :
				public uuid_base {

			public:

				_token(
					__in_opt token_t type = TOKEN_BEGIN,
					__in_opt uint32_t subtype = TOKEN_SUBTYPE_INVALID
					);

				_token(
					__in const _token &other
					);

				virtual ~_token(void);

				_token &operator=(
					__in const _token &other
					);

				virtual void clear(void);

				size_t get_line(void);

				std::string &get_source(void);

				uint32_t get_subtype(void);

				std::string &get_text(void);

				token_t get_type(void);

				double get_value(void);

				bool has_source(void);

				bool has_text(void);

				void set_source(
					__in const std::string &source,
					__in_opt size_t line = 0
					);

				void set_text(
					__in const std::string &text
					);

				void set_type(
					__in token_t type,
					__in_opt uint32_t subtype = TOKEN_SUBTYPE_INVALID
					);

				void set_value(
					__in double value
					);

				virtual std::string to_string(
					__in bool verbose = false
					);

			protected:

				static std::string find_subtype_string(
					__in token_t &type,
					__in uint32_t subtype
					);

				size_t m_line;

				std::string m_source;

				uint32_t m_subtype;

				std::string m_text;

				token_t m_type;

				double m_value;

			private:

				std::recursive_mutex m_lock;

		} token, *token_ptr;

		void token_factory_destroy(void);

		typedef class _token_factory {

			public:

				static _token_factory *acquire(
					__in_opt generic_cb_t dest = NULL
					);

				void add_token(
					__in const uuid &id,
					__in const token &tok
					);

				void clear(void);

				void destroy(void);

				token &generate(
					__in_opt token_t type = TOKEN_BEGIN,
					__in_opt uint32_t subtype = TOKEN_SUBTYPE_INVALID
					);

				token &get_token(
					__in const uuid &id
					);

				bool has_token(
					__in const uuid &id
					);

				void initialize(void);

				bool is_initialized(void);

				void remove_token(
					__in const uuid &id
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				friend void token_factory_destroy(void);

				_token_factory(
					__in_opt generic_cb_t dest = NULL
					);

				_token_factory(
					__in _token_factory &other
					);

				~_token_factory(void);

				_token_factory &operator=(
					__in _token_factory &other
					);

				std::map<uuid, token>::iterator find_token(
					__in const uuid &id
					);

				bool m_initialized;

				static _token_factory *m_instance;

				std::map<uuid, token> m_token_map;

			private:

				std::recursive_mutex m_lock;

		} token_factory, *token_factory_ptr;
	}
}

#endif // LUNA_TOKEN_H_
