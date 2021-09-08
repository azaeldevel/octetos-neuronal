
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

#include "network.hh"
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
	
	
	oct::neu::Line<double> line1(0.43,3.6,0.36,100,0,50);
	//line1.print();
	//line1.dating();
	//line1.plot();
	/*oct::neu::Network<double>::Learning learnig;
	learnig.ratio = 1.0e-8;
	learnig.dEdR = 1.0e-6;
	learnig.iterations = 2000;
	oct::neu::Topology topology(8,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=18;
	topology[2].height=30;
	topology[3].height=30;
	topology[4].height=80;
	topology[5].height=20;
	topology[6].height=5;
	topology[7].height=1;*/
	/*oct::neu::Topology topology(6,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=18;
	topology[2].height=60;
	topology[3].height=30;
	topology[4].height=5;
	topology[5].height=1;*/
	/*oct::neu::Network<double>::Learning learnig;
	learnig.ratio = 1.6e-6;
	learnig.dEdR = 0.1;
	learnig.iterations = 10000;
	oct::neu::Topology topology(4,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=8;
	topology[2].height=8;
	topology[3].height=1;*/
	oct::neu::Network<double>::Learning learnig;
	learnig.ratio = 1.0e-6;
	learnig.dEdR = 0.1;
	learnig.iterations = 10000;
	oct::neu::Topology topology(3,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=2;
	topology[2].height=1;
	oct::neu::Network<double> network(topology,2,1);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	//std::cout << "\n";
	oct::math::Plotter plot;
	oct::math::Plotter plotGraph;
	plot.set_noautotitles();
	for(unsigned int i = 0 ; i < 3; i++)
	{
		network.bp(line1,learnig,&plot);
	}
	double out;
	unsigned int counFail = 0;
	for(oct::neu::Data<double>& d : line1)
	{
		out = *network.spread(d.inputs)[0];
		if(out < 0.1 and d.outputs[0] < 0.1) 
		{
		}
		else if(out > 0.9 and d.outputs[0] > 0.9) 
		{
		}
		else
		{
			counFail++;
			std::cout << "Fallo(" << counFail << ") en ";
			oct::neu::print(d.inputs);
			std::cout << ", la prediccion es " << out << ", sin embargo el valor esperado es ";
			std::cout << d.outputs[0];
			std::cout << "\n";
		}
	}
	return 0;
}

