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

#ifndef LUNA_VERSION_H_
#define LUNA_VERSION_H_

namespace LUNA_NS {

	#define LUNA_TITLE "LUNA"
	#define LUNA_COPYRIGHT "Copyright (C) 2014 David Jolly"

	#define LUNA_VER_MAJOR 0
	#define LUNA_VER_MINOR 1
	#define LUNA_VER_WORK 1433
	#define LUNA_VER_REV 2

	#define LUNA_VER_STR\
		CONCAT_STR(LUNA_VER_MAJOR) "." CONCAT_STR(LUNA_VER_MINOR) "."\
		CONCAT_STR(LUNA_VER_WORK)
	#define LUNA_VER_STR_RES LUNA_VER_STR "." LUNA_VER_REV
	#define LUNA_VER_STR_VERB\
		LUNA_VER_STR " (rev. " CONCAT_STR(LUNA_VER_REV) ", " __DATE__ ", "\
		__TIME__ ")"
}

#endif // LUNA_VERSION_H_
