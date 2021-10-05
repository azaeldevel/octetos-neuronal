
#include <octetos/core/shell.hh>

#include "network.hh"
#include "Line.hh"

int main()
{	
	//oct::neu::Line<double> line(1,1,10,10,0.3,100,1);
	//oct::neu::Line<double> line(1,1,10,10,0.3,100,-1);
	oct::neu::Line<double> line(0,0,1,1,0.03,100,0);
	
	/*oct::neu::Topology topology(oct::neu::ActivationFuntion::SIGMOIDEA,8,10,2,1);	
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 1.0e-3;
	learnig.mE = 0.05;
	//learnig.variable = true;
	learnig.iterations = 1000;*/
	oct::neu::Topology topology(oct::neu::ActivationFuntion::SIGMOIDEA,5,5,2,1);
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 0.25;
	learnig.mE = 0.05;
	learnig.variable = false;
	learnig.iterations = 1000;
	/*oct::neu::Topology topology(oct::neu::ActivationFuntion::SIGMOIDEA,3,5,2,1);
	oct::neu::Learning<double> learnig;	
	learnig.ratio = 1.0e-2;
	learnig.mE = 0.05;
	learnig.iterations = 500;*/
	
	oct::neu::Network network(topology,2,1);
	//std::vector<std::vector<double>*> ds;
	//ds.push_back(&data);
	//std::cout << "\n";
	oct::neu::Plotting<double> plotting;
	//oct::math::Plotter plotGraph;
	plotting.plotter.set_noautotitles();
	plotting.filename = "neronal.dat";
	std::string wintitle = "mean Error ";
	wintitle = wintitle + " : " + std::to_string(0);
	plotting.plotter.set_title(wintitle);
	std::ofstream dat;
	oct::core::Shell shell;
	if(shell.exists(plotting.filename)) shell.rm(plotting.filename);
	
	if(not network.trainig(line,learnig,&plotting,5))
	{
		std::cout << "No se entreno correctamente\n";
		return EXIT_FAILURE;
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
	
	return EXIT_SUCCESS;
}

