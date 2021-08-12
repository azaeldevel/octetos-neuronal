
#ifndef OCTETOS_NEURONAL_HH
#define OCTETOS_NEURONAL_HH

#include <vector>
#include <list>
#include <octetos/math/Vector.hh>

namespace oct::neu
{

typedef double datatype;


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
	
	std::vector<datatype*>& get_inputs();
	oct::math::Vector<datatype>& get_weight();
	datatype& get_out();

	void set_inputs(const oct::math::Vector<datatype*>&);

	datatype spread(datatype (*F)(datatype));
	void resize(unsigned short);
	
private:
	std::vector<datatype*> inputs;
	oct::math::Vector<datatype> weight;
	datatype out;
	//datatype umbral;

private:
	
	datatype sigma();
};


class Layer : public std::vector<Perceptron>
{
public:
	Layer();
	/**
	*\param inputsP Entradas por perceptron
	*\param countP cantidad de perseptornes en la capa
	*\param FA funcion de activavion
	*/
	Layer(unsigned short inputsP, unsigned short countP, datatype (*FA)(datatype));
	void set(unsigned short inputsP, unsigned short countP, datatype (*FA)(datatype));

	oct::math::Vector<datatype*>& get_outputs();
	oct::math::Vector<datatype>& get_gradient();
	oct::math::Vector<datatype>& get_gradient_unit();
	//oct::math::Vector<datatype>& get_gradient_descent();
	static void print(const oct::math::Vector<datatype>&);
	static void print(const oct::math::Vector<datatype*>&);

	void spread();
	void minimize(unsigned short maxit, datatype ratio, oct::math::Vector<datatype>& out, Layer& prevL);

private:
	datatype (*FA)(datatype);

private:
	oct::math::Vector<datatype*> outputs;
	oct::math::Vector<datatype> gradient;
	oct::math::Vector<datatype> gradient_unit;
	//oct::math::Vector<datatype> gradient_descent;
};

typedef std::vector<unsigned short> LayerWidth;

typedef oct::math::Vector<oct::math::Vector<datatype>*> Datas;

/**
*\brief red neuronal
*/
class Network : public std::vector<Layer>
{
public:
	/**
	*\param layerWidth Inidca la caxntidad de neuronal para la capa i-esima, deve de tener 1 para la primer capa
	*\param FA Funcion de activacion
	*\param insP Inidca la canitdad de entradas de cada neurona
	*/
	Network(const LayerWidth& layerWidth,datatype (*FA)(datatype),unsigned short insP,unsigned short outsP);
	~Network();
	oct::math::Vector<datatype*>& spread(oct::math::Vector<datatype>&);
	void learning(const Datas&);

private:
	void conecting();
	void errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l);

private:
	//Dendrities dendrities;
	LayerWidth layerWidth;
	datatype (*FA)(datatype);
	unsigned short inputsPerpceptron;
	unsigned short outsPerpceptron;
	oct::math::Vector<datatype*>* outs;
};

}

#endif