
#include "Line.hh"
#include "perceptron.hh"
#include "plot.hh"

using namespace oct::neu;

int main()
{
	oct::neu::Perceptron<double> perceptron(1);
	oct::neu::Perceptron<double> perceptron2(1);
	std::ofstream file("exp-perceptron-line.dat");	
	unsigned int amoung = 100;
	std::vector<double> dataset;
	dataset.resize(amoung);
	double m = oct::core::randNumber(-1.0,1.0);
	for(unsigned int i = 0; i < amoung; i++)
	{
		dataset[i] = oct::core::randNumber(-1.0,1.0);
		perceptron.inputs[0] = &dataset[i];
		perceptron.weight[0] = m;
		perceptron.spread(oct::neu::ActivationFuntion::SIGMOIDEA);
		oct::math::Plotter::save(file,dataset[i],perceptron.out);
		//perceptron2.inputs[0] = &perceptron.out;
		//perceptron2.weight[0] = 0.9;
		//perceptron2.spread(oct::neu::ActivationFuntion::SIGMOIDEA);
		//oct::math::Plotter::save(file,dataset[i],perceptron.out,perceptron2.out);
	}
	file.flush();
	file.close();
	
	return EXIT_SUCCESS;
}