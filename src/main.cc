
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
	
	
	oct::neu::Line<double> line1(0.43,3.6,0.36,500,0,50);
	//line1.print();
	//line1.dating();
	//line1.plot();
	oct::neu::Topology topology(6);
	//oct::neu::LayerWidth layerWidth(6);
	topology[0].height=2;
	topology[0].AF = oct::neu::ActivationFuntion::IDENTITY;
	topology[1].height=18;
	topology[1].AF = oct::neu::ActivationFuntion::SIGMOIDEA;
	topology[2].height=60;
	topology[2].AF = oct::neu::ActivationFuntion::SIGMOIDEA;
	topology[3].height=30;
	topology[3].AF = oct::neu::ActivationFuntion::SIGMOIDEA;
	topology[4].height=5;
	topology[4].AF = oct::neu::ActivationFuntion::SIGMOIDEA;
	topology[5].height=1;
	topology[5].AF = oct::neu::ActivationFuntion::SIGMOIDEA;
	oct::neu::Network<double> network(topology,6,1,oct::neu::ActivationFuntion::SIGMOIDEA);
	//std::vector<oct::neu::datatype*>& out = network.spread(data);
	//oct::neu::Layer::print(out);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	//std::cout << "\n";
	network.bp(line1,10,0.01);
	
	
	
	return 0;
}

