
#include "Line.hh"
#include "perceptron.hh"
#include "plot.hh"

using namespace oct::neu;

int main()
{
	oct::neu::Perceptron<double> perceptron(2);
	std::ofstream file("exp-perceptron-line.dat");	
	unsigned int amoung = 100;
	std::vector<std::vector<double>> dataset;
	dataset.resize(amoung);
	double epsilon = oct::core::randNumber(-0.01,0.01);
	double m = oct::core::randNumber(-1.0,1.0);
	for(unsigned int i = 0; i < amoung; i++)
	{
		dataset[i].resize(2);
		dataset[i][0] = oct::core::randNumber(-1.0,1.0);
		dataset[i][1] = oct::core::randNumber(-0.01,0.01);
		perceptron.inputs[0] = &dataset[i][0];
		perceptron.inputs[1] = &dataset[i][1];
		perceptron.weight[0] = m;
		perceptron.weight[1] = oct::core::randNumber(-0.01,0.01);
		perceptron.spread(oct::neu::ActivationFuntion::SIGMOIDEA);
		oct::math::Plotter::save(file,dataset[i][0],dataset[i][1],perceptron.out);
	}	
	file.flush();
	file.close();
	
	
}