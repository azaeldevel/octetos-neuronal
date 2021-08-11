
#ifndef OCTETOS_NEURONAL_PERCEPTRON_HH
#define OCTETOS_NEURONAL_PERCEPTRON_HH

#include <vector>

namespace oct::neu
{
typedef float datatype;


datatype sigmoide(datatype);

class Perceptron
{
public:
	Perceptron(unsigned short inputs, datatype umbral, datatype (*F)(datatype));
	
	std::vector<datatype>& get_values();
	std::vector<datatype>& get_weight();
	datatype& get_out();

	void spread();
	
private:
	std::vector<datatype> values;
	std::vector<datatype> weight;
	datatype out;
	datatype umbral;
	datatype (*F)(datatype);

private:
	
	datatype sigma();
};

}

#endif