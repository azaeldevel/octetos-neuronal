
#include "Line.hh"
#include "perceptron.hh"
#include "plot.hh"

using namespace oct::neu;

int main()
{
	oct::neu::Perceptron<double> perceptron(2);
	perceptron.weight[0] = 0.2;
	perceptron.weight[1] = 0.8;
	std::ofstream file("exp-perceptron.dat");	
	unsigned int amoung = 100;
	std::vector<std::vector<double>> dataset;
	dataset.resize(amoung);
	for(unsigned int i = 0; i < amoung; i++)
	{
		dataset[i].resize(2);
		dataset[i][0] = oct::core::randNumber(0.0,1.0);
		dataset[i][1] = oct::core::randNumber(0.0,1.0);
		perceptron.inputs[0] = &dataset[i][0];
		perceptron.inputs[1] = &dataset[i][1];
		perceptron.spread(oct::neu::ActivationFuntion::SIGMOIDEA);
		oct::math::Plotter::save(file,dataset[i][0],dataset[i][1],perceptron.out);
	}	
	file.flush();
	file.close();
	
	
}