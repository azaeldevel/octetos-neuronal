
#ifndef OCTETOS_NEURONAL_NETWORK_HH
#define OCTETOS_NEURONAL_NETWORK_HH

#include <Layer.hh>
#include <list>

namespace oct::neu
{
	//typedef std::vector<unsigned short> LayerWidth;
	
	struct Topology : public std::vector<Model>
	{
		Topology(Index size);
	};

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
		Network(const Topology& t,unsigned short insP,unsigned short outsP) 
		: inputsPerpceptron(insP),outsPerpceptron(outsP),topology(t)
		{
			//std::cout << "Network::Network(---) = step 1\n";
			//validacion de entradas
			for(unsigned short i = 2; i < topology.size(); i++)
			{
				if(topology[i].height * insP < topology[i-1].height) errorToMuchInputsRequiered(i,__FILE__,__LINE__);
			}
			//layerWidth.resize(topology.size());
			/*for(unsigned short i = 0; i < topology.size(); i++)
			{
				layerWidth[i] = lw[i];
			}*/
			//std::cout << "Network::Network(---) = step 2\n";
			
			std::vector<Layer<T>>::resize(topology.size());
			
			//std::cout << "Network::Network(---) = step 3 - " << std::vector<Layer<T>>::size() << "\n";
			std::vector<Layer<T>>::at(0).set(1,topology[0].height,topology[0].AF);
			//std::cout << "Network::Network(---) = step 3.1 - " << 0 << " - " << std::vector<Layer<T>>::at(0).size() << "\n";
			for(unsigned short i = 1; i < topology.size(); i++)
			{
				std::vector<Layer<T>>::at(i).set(insP,topology[i].height,topology[i].AF);//configurar cada capa
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
		void bp(const std::vector<Data<T>>& datas,unsigned short maxit, T ratio)
		{
			//std::cout << "\tvoid Network::bp(..) : step 1\n";
			Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
			outs = &std::vector<Layer<T>>::at(lastlayer).get_outputs();
			//std::cout << "\tvoid Network::bp(..) : step 2\n";
			for(Index indexData = 0; indexData < datas.size(); indexData++)
			{
				for(Index it = 0; it < maxit; it++)
				{
					//std::cout << "\tvoid Network::bp(..) : step 2.1\n";
					std::cout << ">>Data :";
					Layer<T>::print(datas[indexData].inputs);
					if(datas[indexData].outputs[0] > 0.5) std::cout << " - Aceptado";
					else std::cout << " - Rechazado";
					std::cout << "\n";
					std::cout << "prev :";
					Layer<T>::print(*outs);
					std::cout << "\n";
					
					spread(datas[indexData].inputs);

					dEdR.resize(std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs().size());								
					//derivada partcial respecto a la funcion de activacion			
					for(Index i = 0; i < dEdR.size(); i++)
					{
						dEdR[i] = *std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs()[i] - datas[indexData].outputs[i];
					}
			
					for(Index indexLayer = lastlayer; indexLayer > 0; indexLayer--)
					{
						//std::cout << ">>>>Capa :" << j << "\n";
						//std::cout << "\tvoid Network::bp(..) : step 2.1.1\n";
						dEdW.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						dRdZ.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						dEdZ.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						dZdW.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						for(Index l = 0; l < dZdW.size(); l++)
						{
							dZdW[l].resize(std::vector<Layer<T>>::at(indexLayer).at(l).get_weight().size());
						}
						//std::cout << "\tvoid Network::bp(..) : step 2.1.2\n";						
					
						//std::cout << "\tvoid Network::bp(..) : step 2.1.3\n";
						//derivada de la activacion respecto a la suman ponderada
						for(Index i = 0; i < dRdZ.size(); i++)
						{
							switch(topology[indexLayer].AF)
							{
								case ActivationFuntion::SIGMOIDEA:
									//std::cout << "sigma : " << std::vector<Perceptron<T>>::at(i).get_sigma() << "\n";
									dRdZ[i] =  Perceptron<T>::sigmoide_D(std::vector<Layer<T>>::at(indexLayer).at(i).get_sigma());
								break;
								case ActivationFuntion::IDENTITY:
									//dRdZ[i] =  Perceptron<T>::identity_D(std::vector<Perceptron<T>>::at(i).get_sigma());
									dRdZ[i] = T(1);
								break;
								default:
									throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
							};
						}

						//std::cout << "\tvoid Network::bp(..) : step 2.1.4\n";
						//la derivada parcial de  la suman ponderada respecto de los pesos
						for(Index i = 0; i < dZdW.size(); i++)
						{
							//if(dZdW.size() != std::vector<Perceptron<T>>::at(i).get_inputs().size()) dZdW.resize(std::vector<Perceptron<T>>::at(i).get_inputs().size());
							std::vector<Layer<T>>::at(indexLayer).at(i).derivade(dZdW[i]);
						}
						
						//std::cout << "\tvoid Network::bp(..) : step 2.1.5\n";
						//derivada parcial del error repecto de los pesos
						for(Index i = 0; i < dEdW.size(); i++)
						{
							for(Index k = 0; k < dZdW[i].size(); k++)
							{
								dRdZ[i] += dRdZ[i] * dZdW[i][k];
							}	
							dEdW[i] =  dEdR[i] * dRdZ[i];
						}
						
						//std::cout << "\tvoid Network::bp(..) : step 2.1.6\n";
						//error imputado
						for(Index i = 0; i < dEdZ.size(); i++)
						{
							dEdZ[i] =  dEdR[i] * dRdZ[i];
						}
						//std::cout << "dEdR = ";
						//Layer<T>::print(dEdR);
						//std::cout << "\n";
						//std::cout << "dRdZ = ";
						//Layer<T>::print(dRdZ);
						//std::cout << "\n";
						std::cout << "dEdW = ";
						Layer<T>::print(dEdW);
						std::cout << "\n";
						//Layer<T>::print(Layer<T>::at(highIndex).get_inputs());
						//std::cout << "\n";
						//std::cout << "weight : ";
						//Layer<T>::print(Layer<T>::at(highIndex).get_weight());
						//std::cout << "\n";
						//std::cout << "outputs : ";
						//Layer<T>::print(outputs);
						//std::cout << "\n";
						//std::cout << "\tvoid Network::bp(..) : step 2.1.7\n";
						Index highIndex = max(dEdZ);
						//std::cout << "\tvoid Network::bp(..) : step 2.1.8\n";
						std::vector<Layer<T>>::at(indexLayer).at(highIndex).bp(ratio,dEdW[highIndex]);//aplicando el algoritmo de back-propagation a la capa i-esima
						//std::cout << "\tvoid Network::bp(..) : step 2.1.9\n";
					}
					std::cout << "post :";
					Layer<T>::print(*outs);
					std::cout << "\n";
					std::cout << "\n";
				}
			}
			//std::cout << "\tvoid Network::bp(..) : step 3\n";
		}

		Index max(std::vector<T>& data)
		{
			if(data.empty()) throw oct::core::Exception("Arreglo vacio",__FILE__,__LINE__);
			Index maxIndex = 0;
			T maxFound = data[maxIndex];
			for(Index i = 0; i < data.size(); i++)
			{
				if(data[i] > maxFound) 
				{
					maxFound = data[i];
					maxIndex = i;
				}
			}

			return maxIndex;
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
			msg += std::to_string(i) + "' soporta un maximo de " + std::to_string(topology[i].height * inputsPerpceptron) + ", sin embargo la capa '";
			msg += std::to_string(i-1) + "' requiere " + std::to_string(topology[i-1].height);
			throw octetos::core::Exception(msg,f,l);
		}

	private:
		//Dendrities dendrities;
		//LayerWidth layerWidth;
		unsigned short inputsPerpceptron;
		unsigned short outsPerpceptron;
		std::vector<T*>* outs;
		std::vector<T> ins;
		//ActivationFuntion AF;
		const Topology& topology;
		std::vector<T> dEdR,dRdZ,dEdW,dEdZ;
		std::vector<std::vector<T>> dZdW;
	};
}

#endif