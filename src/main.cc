
/*
 * main.cc
 * Copyright (C) 2021 Azael R. <azael.devel@gmail.com>
 * 
 * octetos-neuronal is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * octetos-neuronal is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "Perceptron.hh"


int main()
{
	std::cout << "Hello world!" << std::endl;
	
	oct::neu::Perceptron per(3,2.5,oct::neu::sigmoide);
	per.get_values()[0] = 3.0;
	per.get_values()[1] = 4.0;
	per.get_values()[2] = -2.0;
	per.get_weight()[0] = 0.2;
	per.get_weight()[1] = 0.6;
	per.get_weight()[2] = 0.01;
	per.spread();
	std::cout << "Salida = " << per.get_out() << std::endl;	
	
	
	return 0;
}

