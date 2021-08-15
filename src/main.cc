
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

#include "Network.hh"
#include "Line.hh"

int main()
{
	//std::cout << "Hello world!" << std::endl;
	
	std::vector<double> data(3);
	data[0] = 3.0;
	data[1] = 4.0;
	data[2] = -2.0;
	oct::neu::Perceptron<double> per(3);
	per.get_inputs()[0] = &data[0];
	per.get_inputs()[1] = &data[1];
	per.get_inputs()[2] = &data[2];
	per.get_weight()[0] = 0.2;
	per.get_weight()[1] = 0.6;
	per.get_weight()[2] = 0.01;
	per.spread(oct::neu::ActivationFuntion::SIGMOIDEA);
	std::cout << "Salida = " << per.get_out() << std::endl;	
	
	std::vector<double> value(3);
	value[0] = 3.0;
	value[1] = 4.0;
	value[2] = -2.0;
	oct::neu::Layer<double> layer(3,50,oct::neu::ActivationFuntion::SIGMOIDEA);
	//layer.spread(data);
	//oct::neu::Layer::print(layer.get_gradient());
	
	oct::neu::Line<double> line1(0.43,3.6,0.5,0.3,500,0,50);
	//line1.print();
	//line1.dating();
	//line1.plot();
	oct::neu::LayerWidth layerWidth(6);
	layerWidth[0]=2;
	layerWidth[1]=18;
	layerWidth[2]=60;
	layerWidth[3]=30;
	layerWidth[4]=5;
	layerWidth[5]=1;
	oct::neu::Network<double> network(layerWidth,6,1,oct::neu::ActivationFuntion::SIGMOIDEA);
	//std::vector<oct::neu::datatype*>& out = network.spread(data);
	//oct::neu::Layer::print(out);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	//std::cout << "\n";
	network.bp(line1,10,0.09);
	
	
	
	return 0;
}

