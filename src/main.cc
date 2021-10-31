
#include <octetos/core/shell.hh>

#include "network.hh"
#include "Line.hh"

int main()
{	
	//oct::neu::Line<double> line(1,1,10,10,0.3,100,1);
	//oct::neu::Line<double> line(1,1,10,10,0.3,100,-1);
	oct::neu::Line<double> line(0,0,1,1,0.05,100,0);
	
	oct::neu::Topology topology(oct::neu::ActivationFuntion::RELU,4,3,2,3);
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 2.0;
	learnig.mE = 0.1;
	learnig.iterations = 1000;
	learnig.training = 5;
	learnig.variable = false;
	oct::neu::Network network(topology,2,3);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	std::cout << "\n";
	oct::neu::Plotting<double> plotting;
	//oct::math::Plotter plotGraph;
	plotting.plotter.set_noautotitles();
	plotting.filename = "neuronal";
	std::string wintitle = "mean Error ";
	wintitle = wintitle + " : " + std::to_string(0);
	plotting.plotter.set_title(wintitle);
	std::ofstream dat;
	oct::core::Shell shell;
	//if(shell.exists(plotting.filename)) shell.rm(plotting.filename);
	//std::cout << "Step 1.0\n";
	if(not network.trainig(line,learnig,&plotting))
	{
		std::cout << "No se entreno correctamente\n";
		return EXIT_FAILURE;
	}
	//std::cout << "Step 2.0\n";
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
	
	return EXIT_SUCCESS;
}

