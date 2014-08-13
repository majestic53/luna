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

int
main(void) 
{
	int result = 0;
	luna_ptr inst = NULL;

	TRACE_ENTRY();

	std::cout << luna::version(true, true, true) << std::endl;

	try {

		inst = luna::acquire();
		if(!inst) {
			result = 1;
			goto exit;
		}
		
		inst->initialize();
		std::cout << std::endl << "---" << std::endl;

		// TODO: test here
		parser par("../../../test/test.lu", true);

		while(par.has_next_statement()) {
			std::cout << par.to_string() << std::endl;
			par.move_next_statement();
		}

		std::cout << par.to_string() << std::endl;

		/*while(par.has_previous_token()) {
			par.move_previous_token();
			std::cout << par.to_string() << std::endl;
		}*/
		// ---

		//std::cout << "---" << std::endl << std::endl << inst->to_string(true);
		inst->destroy();
	} catch(std::runtime_error &exc) {
		std::cerr << exc.what() << std::endl;
		result = 1;
	}

exit:
	TRACE_EXIT("Return Value: 0x%x", result);
	std::cin.get();
	return result;
}
