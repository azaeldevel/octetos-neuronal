
#ifndef OCTETOS_NEURONAL_PERCEPTRON_HH
#define OCTETOS_NEURONAL_PERCEPTRON_HH

#include <vector>
#include <list>

namespace oct::neu
{
typedef float datatype;


datatype sigmoide(datatype);
datatype sigmoide_dev(datatype);

datatype gradientDecent(datatype);

struct Case
{
	std::vector<datatype> input;
	datatype out;
};

typedef std::list<Case> Cases;

class Perceptron
{
public:
	Perceptron();
	Perceptron(unsigned short inputs);
	void set(unsigned short inputs);
	
	std::vector<datatype>& get_values();
	std::vector<datatype>& get_weight();
	datatype& get_out();

	void set_values(const std::vector<datatype>&);

	datatype spread(datatype (*F)(datatype));
	
private:
	std::vector<datatype> values;
	std::vector<datatype> weight;
	datatype out;
	//datatype umbral;

private:
	
	datatype sigma();
};


class Layer : public std::vector<Perceptron>
{
public:
	Layer(unsigned short inputsP, unsigned short countP, datatype (*FA)(datatype));
	void learning(const Cases&);

	std::vector<datatype>& get_gradient();
	std::vector<datatype>& get_gradient_unit();
	std::vector<datatype>& get_gradient_descent();
	static void print(const std::vector<datatype>&);

	void spread(const std::vector<datatype>& c);

private:
	datatype (*FA)(datatype);

private:
	std::vector<datatype> gradient;
	std::vector<datatype> gradient_unit;
	std::vector<datatype> gradient_descent;
};

class Network : public std::vector<Layer>
{
public:
	Network(unsigned short inputs, std::vector<unsigned short> layerWidth,unsigned short outputs);
	
private:

};

}

#endif