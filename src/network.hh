
#ifndef OCTETOS_NEURONAL_NETWORK_HH
#define OCTETOS_NEURONAL_NETWORK_HH

#include <layer.hh>
#include <list>

#include "plot.hh"

#define LAYER(L) std::vector<Layer<DATATYPE>>::at(L)
#define NEURONA(L,N) std::vector<Layer<DATATYPE>>::at(L)[N]
#define WEIGHT(L,N,W) std::vector<Layer<DATATYPE>>::at(L)[N].weight[W]
#define INPUT(L,N,I) std::vector<Layer<DATATYPE>>::at(L)[N].inputs[I]

namespace oct::neu
{
	//typedef std::vector<unsigned short> LayerWidth;
	
	struct Topology : public std::vector<Model>
	{
		//float inputsNeurona;
		Topology(Index size);
		/**
		*\brief
		*\param AF Funcion de activacion
		*\param width cantiad, cpa de antrada mas capaz ocultas mas capa de salida
		*\param heightLayer neuronas por capa
		*\param inputs catidad de entradas
		*\param outputs canida de salidas
		**/
		Topology(ActivationFuntion AF,Index width, Index heightLayer, unsigned int inputs, unsigned int outputs);
		Topology(const Topology&);
	};


	/**
	*\brief Red Neuroanl
	*/	
	class Network : public std::vector<Layer<DATATYPE>>
	{
	public:	
		
	public:
		/**
		*\param layerWidth Inidca la caxntidad de neuronal para la capa i-esima, deve de tener 1 para la primer capa
		*\param FA Funcion de activacion
		*\param insP Inidca la canitdad de entradas de cada neurona
		*/
		Network(const Topology& t,unsigned short countInputs,unsigned short countOutputs);

		const std::vector<DATATYPE*>& spread(const std::vector<DATATYPE>& ds);

		/**
		*\brief Algoritmo de back-propagation
		*\return true si adquirio suficente conocimiento, false de otra forma.
		*/		
		bool bp(const std::vector<Data<DATATYPE>>& datas, Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting);
		bool trainig(const std::vector<Data<DATATYPE>>& datas, Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting);
		bool trainig(const std::vector<Data<DATATYPE>>& datas, Plotting<DATATYPE>* plotting,unsigned int);


		Index max(const Layer<DATATYPE>& layer);
		Index max_weight(const Perceptron<DATATYPE>& perceptron);
		void display(const Data<DATATYPE>& data,oct::math::Plotter* plotGraph, const Learning<DATATYPE>& learning);
		
	private:
		void conecting();
		void errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l);

		
		/**
		*\brief Error cuadratico medio
		*/
		DATATYPE dMSEdR(const std::vector<Data<DATATYPE>>& datas);
		
		/**
		*\brief Error cuadratico medio
		*/
		DATATYPE dMSEdR(const Data<DATATYPE>& data);
		
		/**
		*\brief 
		*/
		real changes_outputlayer(const std::vector<Data<DATATYPE>>& datas);
		
		DATATYPE dRdZ(Index layer,Index neurona);

	private:
		//Dendrities dendrities;
		//LayerWidth layerWidth;
		unsigned short inputsPerpceptron;
		unsigned short outsPerpceptron;
		std::vector<DATATYPE*>* outs;
		std::vector<DATATYPE> ins;
		//ActivationFuntion AF;
		Topology topology;
	};
}

#endif