

#include "network.hh"
#include "Line.hh"

int main()
{
	//std::cout << "Hello world!" << std::endl;
	
	std::vector<double> data(3);
	data[0] = 3.0;
	data[1] = 4.0;
	data[2] = -2.0;
	oct::neu::Neurona<double> per(3);
	per.get_inputs()[0] = &data[0];
	per.get_inputs()[1] = &data[1];
	per.get_inputs()[2] = &data[2];
	per.get_weight()[0] = 0.2;
	per.get_weight()[1] = 0.6;
	per.get_weight()[2] = 0.01;
	per.spread(oct::neu::ActivationFuntion::SIGMOIDEA);
	std::cout << "Salida = " << per.get_out() << std::endl;	
	
	
	//oct::neu::Line<double> line(1,1,10,10,0.3,100,1);
	//oct::neu::Line<double> line(1,1,10,10,0.3,100,-1);
	oct::neu::Line<double> line(1,1,10,10,0.3,3000,0);
	
	/*oct::neu::Topology topology(oct::neu::ActivationFuntion::SIGMOIDEA,8,10,2,1);	
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 1.0e-3;
	learnig.mE = 0.05;
	//learnig.variable = true;
	learnig.iterations = 1000;*/
	oct::neu::Topology topology(oct::neu::ActivationFuntion::SIGMOIDEA,5,5,2,1);
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 1.0e-5;
	learnig.mE = 0.005;
	learnig.iterations = 500;
	/*oct::neu::Topology topology(oct::neu::ActivationFuntion::SIGMOIDEA,3,5,2,1);
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 1.0e-2;
	learnig.mE = 0.05;
	learnig.iterations = 500;*/
	
	oct::neu::Network<double> network(topology,2,1);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	//std::cout << "\n";
	oct::neu::Plotting<double> plotting;
	//oct::math::Plotter plotGraph;
	plotting.plotter.set_noautotitles();
	std::ofstream dat;
	
	//for(unsigned int i = 0;i < 3; i++)
	{
		std::string fnNeuranl = "neuronal-0.dat";
		dat.open(fnNeuranl);
		for(oct::neu::Data<double>& d : line)
		{
			double out = *network.spread(d.inputs)[0];
			oct::math::Plotter::save(dat,d.inputs[0],d.inputs[1], out);
		}
		dat.flush();
		dat.close();
				
		std::string wintitle = "mean Error ";
		plotting.plotter.set_title(wintitle);
		network.bp(line,learnig,&plotting);
		
		fnNeuranl = "neuronal-1.dat";
		dat.open(fnNeuranl);
		for(oct::neu::Data<double>& d : line)
		{
			double out = *network.spread(d.inputs)[0];
			oct::math::Plotter::save(dat,d.inputs[0],d.inputs[1], out);
		}
		dat.flush();
		dat.close();
	}
	unsigned int counFail = 0;
	for(oct::neu::Data<double>& d : line)
	{
		double out = *network.spread(d.inputs)[0];
		if(out < 0.5 and d.outputs[0] < 0.5 ) 
		{
		}
		else if(out > 0.5 and d.outputs[0] > 0.5)
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

