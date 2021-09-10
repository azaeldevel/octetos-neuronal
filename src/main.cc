

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
	
	
	//oct::neu::Line<double> line1(1,1,10,10,0.3,100,1);
	//oct::neu::Line<double> line2(1,1,10,10,0.3,100,-1);
	oct::neu::Line<double> line3(1,1,10,10,0.3,100,0);
	oct::neu::Network<double>::Learning learnig;	
	learnig.ratio = 1.0e-1;
	learnig.dEdR = 0.1;
	learnig.iterations = 1000;
	oct::neu::Topology topology(8,oct::neu::ActivationFuntion::SIGMOIDEA,2,5,1);
	/*topology[0].height=2;
	topology[1].height=18;
	topology[2].height=30;
	topology[3].height=30;
	topology[4].height=80;
	topology[5].height=20;
	topology[6].height=5;
	topology[7].height=1;*/
	/*learnig.ratio = 1.0e-5;
	learnig.dEdR = 0.1;
	learnig.iterations = 1000;
	oct::neu::Topology topology(6,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=18;
	topology[2].height=60;
	topology[3].height=30;
	topology[4].height=5;
	topology[5].height=1;*/
	/*learnig.ratio = 1.6e-4;
	learnig.dEdR = 0.1;
	learnig.iterations = 1000;
	oct::neu::Topology topology(4,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=8;
	topology[2].height=8;
	topology[3].height=1;*/
	/*learnig.ratio = 1.0e-1;
	learnig.dEdR = 0.1;
	learnig.iterations = 1000;
	oct::neu::Topology topology(3,oct::neu::ActivationFuntion::SIGMOIDEA);
	topology[0].height=2;
	topology[1].height=3;
	topology[2].height=1;*/
	oct::neu::Network<double> network(topology,2,1);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	//std::cout << "\n";
	oct::neu::Network<double>::Plotting plotting;
	//oct::math::Plotter plotGraph;
	plotting.plotter.set_noautotitles();
	std::ofstream dat;
	unsigned int i = 0;
	/*for(unsigned int i = 0 ; i < 2; i++)
	{
		std::string fnNeuranl = "neuronal-";
		fnNeuranl += std::to_string(i) + ".dat";
		dat.open(fnNeuranl);
		for(oct::neu::Data<double>& d : line1)
		{
			double out = *network.spread(d.inputs)[0];
			oct::math::Plotter::save(dat,d.inputs[0],d.inputs[1], out);
		}
		dat.flush();
		dat.close();
		
		std::string wintitle = "dEdR : ";
		wintitle += std::to_string( i + 1);
		plotting.plotter.set_title(wintitle);
		network.bp(line1,learnig,&plotting);
	}*/
	/*for(unsigned int i = 2 ; i < 4; i++)
	{
		std::string fnNeuranl = "neuronal-";
		fnNeuranl += std::to_string(i) + ".dat";
		dat.open(fnNeuranl);
		for(oct::neu::Data<double>& d : line2)
		{
			double out = *network.spread(d.inputs)[0];
			oct::math::Plotter::save(dat,d.inputs[0],d.inputs[1], out);
		}
		dat.flush();
		dat.close();
		
		std::string wintitle = "dEdR : ";
		wintitle += std::to_string( i + 1);
		plotting.plotter.set_title(wintitle);
		network.bp(line2,learnig,&plotting);
	}*/
	for(; i < 5; i++)
	{
		std::string fnNeuranl = "neuronal-";
		fnNeuranl += std::to_string(i) + ".dat";
		dat.open(fnNeuranl);
		for(oct::neu::Data<double>& d : line3)
		{
			double out = *network.spread(d.inputs)[0];
			oct::math::Plotter::save(dat,d.inputs[0],d.inputs[1], out);
		}
		dat.flush();
		dat.close();
		
		std::string wintitle = "dEdR : ";
		wintitle += std::to_string( i + 1);
		plotting.plotter.set_title(wintitle);
		network.bp(line3,learnig,&plotting);
	}
	double out;
	unsigned int counFail = 0;
	for(oct::neu::Data<double>& d : line3)
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

