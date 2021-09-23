
#include "Line.hh"
#include "perceptron.hh"
#include "plot.hh"

using namespace oct::neu;

int main()
{
	Line<double> line1(1,1,10,10,0.3,100,1);
	Perceptron<double> neurona(2);
	std::ofstream dat;
	oct::math::Plotter plotter;
	Learning<double> learnig;	
	learnig.ratio = 1.0e-1;
	learnig.mE = 0.1;
	learnig.iterations = 100;
	
	for(unsigned int i = 0 ; i < 5; i++)
	{
		std::string fnNeuranl = "neuronal-";
		fnNeuranl += std::to_string(i) + ".dat";
		dat.open(fnNeuranl);
		for(oct::neu::Data<double>& d : line1)
		{
			neurona.get_inputs()[0] = &d.inputs[0];
			neurona.get_inputs()[1] = &d.inputs[1];
			double out = neurona.spread(ActivationFuntion::SIGMOIDEA);
			oct::math::Plotter::save(dat,d.inputs[0],d.inputs[1], out);
		}
		dat.flush();
		dat.close();
		
		std::string wintitle = "Entrenamiento : ";
		wintitle += std::to_string( i + 1);
		plotter.set_title(wintitle);
		neurona.bp(line1,learnig,&plotter);
	}
}