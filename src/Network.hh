
#ifndef OCTETOS_NEURONAL_NETWORK_HH
#define OCTETOS_NEURONAL_NETWORK_HH

#include <Layer.hh>
#include <list>

namespace oct::neu
{
	typedef std::vector<unsigned short> LayerWidth;

	
	/**
	*\brief red neuronal
	*/	
	template<typename T> class Network : public std::vector<Layer<T>>
	{
	public:
		/**
		*\param layerWidth Inidca la caxntidad de neuronal para la capa i-esima, deve de tener 1 para la primer capa
		*\param FA Funcion de activacion
		*\param insP Inidca la canitdad de entradas de cada neurona
		*/
		Network(const LayerWidth& lw,T (*fa)(T),unsigned short insP,unsigned short outsP,T (*d)(T)) 
		: FA(fa),inputsPerpceptron(insP),outsPerpceptron(outsP),D(d)
		{
			//std::cout << "Network::Network(---) = step 1\n";
			//validacion de entradas
			for(unsigned short i = 2; i < lw.size(); i++)
			{
				if(lw[i] * insP < lw[i-1]) errorToMuchInputsRequiered(i,__FILE__,__LINE__);
			}
			layerWidth.resize(lw.size());
			for(unsigned short i = 0; i < lw.size(); i++)
			{
				layerWidth[i] = lw[i];
			}
			//std::cout << "Network::Network(---) = step 2\n";
			
			std::vector<Layer<T>>::resize(layerWidth.size());
			
			//std::cout << "Network::Network(---) = step 3 - " << size() << "\n";
			std::vector<Layer<T>>::at(0).set(1,layerWidth[0],fa,d);
			//std::cout << "Network::Network(---) = step 3.1 - " << 0 << " - " << at(0).size() << "\n";
			for(unsigned short i = 1; i < layerWidth.size(); i++)
			{
				std::vector<Layer<T>>::at(i).set(insP,layerWidth[i],fa,d);//configurar cada capa
				//std::cout << "Network::Network(---) = step 3.1 - " << i << " - " << at(i).size() << "\n";
			}
			
			//std::cout << "Network::Network(---) = step 4\n";
			conecting();
		}
		const std::vector<T*>& spread(const std::vector<T>& ds)
		{
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 1\n";
			if(std::vector<Layer<T>>::at(0).size() != ds.size()) throw oct::core::Exception("La cantidad de entradas en la red no coincide con la cantidad de datos de entradas",__FILE__,__LINE__);
			ins = ds;
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 2\n";
			for(unsigned short i = 0; i < std::vector<Layer<T>>::at(0).size(); i++)
			{
				//std::cout << "\tEntradas por neuraona = " << at(0).at(i).get_inputs().size << "\n";
				if(std::vector<Layer<T>>::at(0).at(i).get_inputs().size() != 1) throw oct::core::Exception("La cantidad de entradas por neurona deve ser 1 en la primer capa.",__FILE__,__LINE__);
				std::vector<Layer<T>>::at(0).at(i).get_inputs()[0] = &ins[i];//se asigna cada dato a cada entrada
			}
			
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3\n";
			for(unsigned short i = 0; i < std::vector<Layer<T>>::size(); i++)
			{
				//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 3.1 - " << i << "\n";
				std::vector<Layer<T>>::at(i).spread();	
			}
			
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 4\n";		
			outs = &(std::vector<Layer<T>>::at(std::vector<Layer<T>>::size() - 1).get_outputs());//se usa la salida de la ultima capa
			
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 5\n";
			return *outs;
		}
		/**
		*\brief Algoritmo de back-propagation
		*/
		void bp(const std::vector<std::vector<T>*>& ds)
		{
			std::vector<T*> pdt;
			for(Index i = 0; i < ds.size(); i++)
			{
				pdt.resize(ds[i]->size());
				for(Index j = 0; j < pdt.size(); j++)
				{
					pdt[j] = &ds[i]->at(j);
				}
				spread(*ds[i]);
				Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
				std::vector<Layer<T>>::at(lastlayer).gd(50,0.01,std::vector<Layer<T>>::at(lastlayer-1),pdt);//aplicando el algoritmo de back-propagation a la capa i-esim
			}
		}

	private:
		void conecting()
		{
			//std::cout << "\n";
			for(unsigned short i = 1; i < std::vector<Layer<T>>::size(); i++)
			{
				//std::cout << "Cantidd de Layers = " << size() << "\n";
				for(unsigned short j = 0; j < std::vector<Layer<T>>::at(i).size(); j++)
				{
					//std::cout << "\tCantidd de neuraonas = " << at(i).size() << "\n";
					for(unsigned short k = 0; k < std::vector<Layer<T>>::at(i).at(j).get_inputs().size(); k++)
					{
						if(k < std::vector<Layer<T>>::at(i-1).size()) 
						{
							std::vector<Layer<T>>::at(i).at(j).get_inputs().at(k) = &(std::vector<Layer<T>>::at(i-1).at(k).get_out());
						}
						else 
						{
							std::vector<Layer<T>>::at(i).at(j).resize(k);
							break;
						}
					}
				}	
			}
		}
		void errorToMuchInputsRequiered(unsigned short i,const char* f, unsigned int l)
		{
			std::string msg = "La capa '";
			msg += std::to_string(i) + "' soporta un maximo de " + std::to_string(layerWidth[i] * inputsPerpceptron) + ", sin embargo la capa '";
			msg += std::to_string(i-1) + "' requiere " + std::to_string(layerWidth[i-1]);
			throw octetos::core::Exception(msg,f,l);
		}

	private:
		//Dendrities dendrities;
		LayerWidth layerWidth;
		T (*FA)(T);
		T (*D)(T);
		unsigned short inputsPerpceptron;
		unsigned short outsPerpceptron;
		std::vector<T*>* outs;
		std::vector<T> ins;
		std::vector<std::vector<T>*> ds;
	};
}

#endif