
#ifndef OCTETOS_NEURONAL_NETWORK_HH
#define OCTETOS_NEURONAL_NETWORK_HH

#include <layer.hh>
#include <list>

#include "plot.hh"


namespace oct::neu
{
	//typedef std::vector<unsigned short> LayerWidth;
	
	struct Topology : public std::vector<Model>
	{
		float inputsNeurona;

		
		Topology(Index size);
		Topology(Index size, ActivationFuntion AF);
		Topology(const Topology&);
	};


	/**
	*\brief red neuronal
	*/	
	template<typename T> class Network : public std::vector<Layer<T>>
	{
	public:	
		struct Learning
		{
			T ratio;
			T dEdR; 
			unsigned int iterations;
		};
		struct Progress
		{
			std::vector<T> dEdR;
		};
	public:
		/**
		*\param layerWidth Inidca la caxntidad de neuronal para la capa i-esima, deve de tener 1 para la primer capa
		*\param FA Funcion de activacion
		*\param insP Inidca la canitdad de entradas de cada neurona
		*/
		Network(const Topology& t,unsigned short countInputs,unsigned short countOutputs) : topology(t)
		{
			//std::cout << "Network::Network(---) = step 1\n";
			if(topology.size() < 3) throw oct::core::Exception("La red deve tener 1 capa de entrada, al menos 1 capa oculta y 1 de salida",__FILE__,__LINE__);
			if(topology.at(0).height != countInputs) throw oct::core::Exception("La cantidad de entradas no coincide",__FILE__,__LINE__);
			//std::cout << topology.at(topology.size() - 1).height << " != " << countOutputs << "\n";
			if(topology.at(topology.size() - 1).height != countOutputs) throw oct::core::Exception("La cantidad de salidas no coincide",__FILE__,__LINE__);
			//std::cout << "Network::Network(---) = step 2\n";
			
			std::vector<Layer<T>>::resize(topology.size());
			
			//std::cout << "Network::Network(---) = step 3 - " << std::vector<Layer<T>>::size() << "\n";
			//cada neurona se conecta con todas las salidas de la capa anterior
			topology.at(0).inputsNeurona = 1;
			std::vector<Layer<T>>::at(0).set(topology.at(0));
			for(unsigned short i = 1; i < topology.size(); i++)
			{
				topology.at(i).inputsNeurona = topology.at(i-1).height;
				std::vector<Layer<T>>::at(i).set(topology.at(i));
			}

			//std::cout << "Network::Network(---) = step 4\n";
			conecting();
		}
		const std::vector<T*>& spread(const std::vector<T>& ds)
		{
			//std::cout << "\tstd::vector<datatype>& Network::spread(std::vector<datatype>& out) : step 1\n";
			//std::cout << std::vector<Layer<T>>::at(0).size() << " != " << ds.size() << "\n";
			if(std::vector<Layer<T>>::at(0).size() != ds.size()) throw oct::core::Exception("Los vectores de datos no coinciden",__FILE__,__LINE__);
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
		void bp(const std::vector<Data<T>>& datas, const Learning& learning, oct::math::Plotter* plotByIt)
		{
			//std::cout << "\tvoid Network::bp(..) : step 1\n";
			Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
			outs = &std::vector<Layer<T>>::at(lastlayer).get_outputs();
			//std::cout << "\tvoid Network::bp(..) : step 2\n";
			std::list<std::vector<T>> errDataPlot;
			std::list<std::vector<T>> fDataPlot;
			if(plotByIt != NULL)
			{
				plotByIt->set_terminal("qt");
				std::string titleplot = "dEdR";
				plotByIt->set_title(titleplot);
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotByIt->set_label(labelRatio,100,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.dEdR);
				plotByIt->set_label(labeldEdR,100,0.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotByIt->set_label(labelCountData,100,0.05);
			}
			dEdR.resize(std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs().size());
			T dEdR_signed = 0;
			for(Index it = 0; it < learning.iterations; it++)
			{
				//std::cout << "\tIteracion : " << it << "\n";;
				T mdEdR_Set = 0;//promedio de las derivadas
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					//std::cout << "\t\tDato : " << indexData << "\n";;
					//std::cout << "\tvoid Network::bp(..) : step 2.1\n";
					//std::cout << "\t>>Data :";
					//std::cout << "\t";
					//Layer<T>::print(datas[indexData].inputs);
					//std::cout << " - ";	
					//Layer<T>::print(datas[indexData].outputs);				
					//std::cout << "\n";
					//std::cout << "\tprev :";
					//Layer<T>::print(*outs);
					//std::cout << "\n";
					
					spread(datas[indexData].inputs);
													
					//derivada partcial respecto a la funcion de activacion			
					for(Index i = 0; i < dEdR.size(); i++)
					{
						//T val = *std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs()[i] - datas[indexData].outputs[i];
						//std::cout << "\tdEdR[i] = " << val << " = " << *std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs()[i] << " - " << datas[indexData].outputs[i] << "\n";
						dEdR[i] = *std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs()[i] - datas[indexData].outputs[i];
					}

					T mdEdR_Data = 0;//promedio de las derivadas
					for(Index i = 0; i < dEdR.size(); i++)
					{
						mdEdR_Data += std::abs(dEdR[i]);
					}
					mdEdR_Data /= T(dEdR.size());
					//std::cout << "\tMedia del dato de dEdR : " << mdEdR_Data << "\n";
					mdEdR_Set += mdEdR_Data;

					for(Index indexLayer = lastlayer; indexLayer > 0; indexLayer--)
					{
						//std::cout << "\t\t>>Capa :" << indexLayer << "\n";
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
							T dRdW = dRdZ[i];
							for(Index k = 0; k < dZdW[i].size(); k++)
							{
								dRdW += dRdZ[i] * dZdW[i][k];
							}	
							dEdW[i] =  dEdR[i] * dRdZ[i] * dRdW;
						}
						
						//std::cout << "\tvoid Network::bp(..) : step 2.1.6\n";
						//error imputado
						for(Index i = 0; i < dEdZ.size(); i++)
						{
							dEdZ[i] =  dEdR[i] * dRdZ[i];
						}
						Index highIndex = max(dEdZ);

						//std::cout << "dRdZ = ";
						//Layer<T>::print(dRdZ);
						//std::cout << "\n";
						//std::cout << "\t\tdEdW = ";
						//Layer<T>::print(dEdW);
						//std::cout << "\n";
						//std::cout << "dEdR = ";
						//Layer<T>::print(dEdR);
						//std::cout << "\n";
						//std::cout << "dZdW = ";
						//Layer<T>::print(dZdW);
						//std::cout << "\n";
						//Layer<T>::print(Layer<T>::at(highIndex).get_inputs());
						//std::cout << "\n";
						//std::cout << "\t\t\tweight : ";
						//Layer<T>::print(Layer<T>::at(indexLayer).get_weight());
						//std::cout << "\n";
						//std::cout << "outputs : ";
						//Layer<T>::print(outputs);
						//std::cout << "\n";
						//std::cout << "\tvoid Network::bp(..) : step 2.1.7\n";
						//std::cout << "\tvoid Network::bp(..) : step 2.1.8\n";
						//std::vector<Layer<T>>::at(indexLayer).at(highIndex).bp(learning.ratio,dEdW[highIndex]);//aplicando el algoritmo de back-propagation a la capa i-esima
						std::vector<Layer<T>>::at(indexLayer).gd(highIndex,learning.ratio,dEdW[highIndex]);
						//std::cout << "\tvoid Network::bp(..) : step 2.1.9\n";
					}
					//std::cout << "\tpost :";
					//Layer<T>::print(*outs);
					//std::cout << " - ";	
					//Layer<T>::print(datas[indexData].outputs);
					//std::cout << "\n";

				}
				mdEdR_Set /= T(datas.size());
				/*if(it == 0)
				{
					if(mdEdR_Set > 0) dEdR_signed = 1.0;
					else if(mdEdR_Set < 0) dEdR_signed = -1.0;
				}*/
				//std::cout << "\tMedia de mdEdR : " << mdEdR_Set << "\n";
				if(mdEdR_Set < learning.dEdR) break;
				/*if( std::abs(mdEdR_Set) < learning.dEdR) 
				{
					break;
				}*/
				//std::cout << "mdEdR = " << mdEdR_Set << "\n";
				if(plotByIt != NULL)
				{
					std::vector<T> vecerr(2);
					vecerr[0] = it;
					vecerr[1] = mdEdR_Set;
					errDataPlot.push_back(vecerr);
					plotByIt->plotting(errDataPlot);
				}
			}
			if(plotByIt != NULL)
			{
				std::string output = "output.svg";
				//output += std::to_string(indexData) + ".svg";
				plotByIt->set_terminal("svg");
				plotByIt->set_output(output);
				plotByIt->plotting(errDataPlot);
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
		void display(const Data<T>& data,oct::math::Plotter* plotGraph, const Learning& learning)
		{

			plotGraph->set_terminal("qt");
			std::string titleplot = "funcion";
			plotGraph->set_title(titleplot);
			std::string labelRatio = "ratio = ";
			labelRatio += std::to_string(learning.ratio);
			plotGraph->set_label(labelRatio,100,0.15);
			std::string labeldEdR = "max dEdR = ";
			labeldEdR += std::to_string(learning.dEdR);
			plotGraph->set_label(labeldEdR,100,0.10);
			if(data.inputs.size() < 2 or data.inputs.size() > 3) throw oct::core::Exception("Dimension no manejable",__FILE__,__LINE__);

			std::vector<T> vecdat(data.inputs.size() + 1);
			if(data.inputs.size() > 1) vecdat[0] = data.inputs[0];
			if(data.inputs.size() > 2) vecdat[1] = data.inputs[1];
			if(std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs().size() == 1) vecdat[2] = *std::vector<Layer<T>>::at(std::vector<Layer<T>>::size()-1).get_outputs()[0];
			else throw oct::core::Exception("Dimension no manejable",__FILE__,__LINE__);
		}
		
	private:
		void conecting()
		{
			//std::cout << "\n";
			for(unsigned short i = 1; i < std::vector<Layer<T>>::size(); i++)//cada capa
			{
				//std::cout << "Cantidd de Layers = " << size() << "\n";
				for(unsigned short j = 0; j < std::vector<Layer<T>>::at(i).size(); j++)//cada neurona de la capa i
				{
					//std::cout << "\tCantidd de neuraonas = " << at(i).size() << "\n";
					for(unsigned short k = 0; k < topology.at(i).inputsNeurona; k++)
					{
						std::vector<Layer<T>>::at(i).at(j).get_inputs().at(k) = &(std::vector<Layer<T>>::at(i-1).at(k).get_out());						
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
		Topology topology;
		std::vector<T> dEdR,dRdZ,dEdW,dEdZ;
		std::vector<std::vector<T>> dZdW;
		std::list<Progress> progress;
	};
}

#endif