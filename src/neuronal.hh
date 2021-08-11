
#ifndef OCTETOS_NEURONAL_PERCEPTRON_HH
#define OCTETOS_NEURONAL_PERCEPTRON_HH

#include <vector>
#include <list>

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
	std::vector<datatype>& get_weight();
	datatype& get_out();

	void set_inputs(const std::vector<datatype*>&);

	datatype spread(datatype (*F)(datatype));
	void resize(unsigned short);
	
private:
	std::vector<datatype*> inputs;
	std::vector<datatype> weight;
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

	std::vector<datatype*>& get_outputs();
	std::vector<datatype>& get_gradient();
	std::vector<datatype>& get_gradient_unit();
	std::vector<datatype>& get_gradient_descent();
	static void print(const std::vector<datatype>&);
	static void print(const std::vector<datatype*>&);

	void spread();

private:
	datatype (*FA)(datatype);

private:
	std::vector<datatype*> outputs;
	std::vector<datatype> gradient;
	std::vector<datatype> gradient_unit;
	std::vector<datatype> gradient_descent;
};

typedef std::vector<unsigned short> LayerWidth;

typedef std::vector<std::vector<datatype>*> Datas;

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
	std::vector<datatype*>& spread(std::vector<datatype>&);
	void learning(const Datas&);

private:
	void conecting();
	void errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l);

private:
	//Dendrities dendrities;
	LayerWidth layerWidth;
	datatype (*FA)(datatype);
	unsigned short inputsPerpceptron;
	datatype*** dendrities;
	unsigned short outsPerpceptron;
	std::vector<datatype*>* outs;
};

}

#endif