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

#ifndef LUNA_NODE_TYPE_H_
#define LUNA_NODE_TYPE_H_

namespace LUNA_NS {

	namespace LUNA_COMP_NS {

		enum {
			LUNA_NODE_EXCEPTION_ALLOC_FAILED = 0,
			LUNA_NODE_EXCEPTION_ID_ALREADY_ADDED,
			LUNA_NODE_EXCEPTION_ID_NOT_FOUND,
			LUNA_NODE_EXCEPTION_INVALID_NODE_POSITION,
			LUNA_NODE_EXCEPTION_UNINITIALIZED,
		};

		static const std::string LUNA_NODE_EXCEPTION_STR[] = {
			"Node factory allocation failed",
			"Node UUID was already added",
			"Node UUID was not found",
			"Invalid node position",
			"Node factory is uninitialized",
			};

		#define LUNA_NODE_EXCEPTION_MAX LUNA_NODE_EXCEPTION_UNINITIALIZED

		#define LUNA_NODE_EXCEPTION_STRING(_EXCEPT_)\
			((_EXCEPT_) > LUNA_NODE_EXCEPTION_MAX ? UNKNOWN_EXCEPTION : LUNA_NODE_EXCEPTION_STR[_EXCEPT_].c_str())
		#define THROW_LUNA_NODE_EXCEPTION(_EXCEPT_)\
			THROW_EXCEPTION(LUNA_NODE_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_LUNA_NODE_EXCEPTION_MESSAGE(_EXCEPT_, _FORMAT_, ...)\
			THROW_EXCEPTION_MESSAGE(LUNA_NODE_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)

		class _node;
		typedef _node node, *node_ptr;

		class _node_factory;
		typedef _node_factory node_factory, *node_factory_ptr;
	}
}

#endif // LUNA_NODE_TYPE_H_
