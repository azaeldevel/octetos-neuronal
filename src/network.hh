
#ifndef OCTETOS_NEURONAL_NETWORK_HH
#define OCTETOS_NEURONAL_NETWORK_HH

#include <layer.hh>
#include <list>

#include "plot.hh"

#define LAYER(L) std::vector<Layer<T>>::at(L)
#define NEURONA(L,N) std::vector<Layer<T>>::at(L).at(N)
#define WEIGHT(L,N,W) std::vector<Layer<T>>::at(L).at(N).weight[W]
#define INPUT(L,N,I) std::vector<Layer<T>>::at(L).at(N).inputs[I]

namespace oct::neu
{
	//typedef std::vector<unsigned short> LayerWidth;
	
	struct Topology : public std::vector<Model>
	{
		//float inputsNeurona;
		Topology(Index size);
		Topology(ActivationFuntion AF,Index width, Index heightLayer, unsigned int inputs, unsigned int outputs);
		Topology(const Topology&);
	};


	/**
	*\brief red neuronal
	*/	
	template<typename T> class Network : public std::vector<Layer<T>>
	{
	public:	
		
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
			//if(topology.size() - 2 == counHidden) throw oct::core::Exception("La cantidad de capaz oculpas es incorrecta.",__FILE__,__LINE__);
			
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
			//print(ins);
			//std::cout << "\n";
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
		*\return true si adquirio suficente conocimiento, false de otra forma.
		*/
		/*bool bp(const std::vector<Data<T>>& datas, const Learning<T>& learning, Plotting<T>* plotting)
		{
			Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
			
			if(plotting != NULL)
			{
				plotting->plotter.set_terminal("qt");
				//std::string titleplot = "dEdR";
				//plotByIt->set_title(titleplot);
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotting->plotter.set_label(labelRatio,10,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.mE);
				plotting->plotter.set_label(labeldEdR,10,0.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotting->plotter.set_label(labelCountData,10,0.05);
			}
			
			for(Index iteration = 0; iteration < learning.iterations; iteration++)
			{
				//
				std::vector<T> E;
				E.resize(LAYER(lastlayer).size());
				T Em = 0;
				for(Index i = 0; i < LAYER(lastlayer).size(); i++)
				{
					E[i] = 0;
				}
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					spread(datas[indexData].inputs);
					for(Index i = 0; i < LAYER(lastlayer).size(); i++)
					{
						E[i] += std::pow(datas[indexData].outputs[i] - (*LAYER(lastlayer).get_outputs()[i]),T(2));
					}
				}
				for(Index i = 0; i < LAYER(lastlayer).size(); i++)
				{
					E[i] /= T(datas.size());
				}
				for(Index i = 0; i < LAYER(lastlayer).size(); i++)
				{
					Em += E[i];
				}
				if(LAYER(lastlayer).size() > 1) Em /= T(LAYER(lastlayer).size());

				//
				if(plotting != NULL)
				{
					plotting->last++;
					std::vector<T> vecerr(2);
					vecerr[0] = plotting->last;
					vecerr[1] = Em;
					plotting->data.push_back(vecerr);
					plotting->plotter.plotting(plotting->data);
				}
				if(learning.mE > Em) return true;


				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					spread(datas[indexData].inputs);
					std::vector<T> So;
					So.resize(std::vector<Layer<T>>::at(lastlayer).get_outputs().size());
					//calculo del error
					for(Index i = 0; i < So.size(); i++)
					{
						So[i] = datas[indexData].outputs[i] - (*std::vector<Layer<T>>::at(lastlayer).get_outputs()[i]);						
						//So[i] = E;
						switch(topology[lastlayer].AF)
						{
								case ActivationFuntion::SIGMOIDEA:
									//std::cout << "sigma : " << NEURONA(lastlayer,i).sigma << "\n";
									So[i] *=  Perceptron<T>::sigmoidea_D(NEURONA(lastlayer,i).result);
								break;
								case ActivationFuntion::IDENTITY:
									//So[i] *= T(1);
								break;
								default:
									throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
						};						
					}

					T Sow = 0;
					for(Index i = 0; i < So.size(); i++)
					{
						for(Index j = 0; j < NEURONA(lastlayer,i).weight.size(); j++)
						{
							Sow += So[i] * WEIGHT(lastlayer,i,j);
						}
					}
					
					std::vector<std::vector<T>> Sh;
					Sh.resize(std::vector<Layer<T>>::size() - 1);
					for(Index indexLayer = lastlayer - 1; indexLayer >= 0; indexLayer--)//capa
					{
						Sh[indexLayer].resize(LAYER(indexLayer).size());
						for(Index i = 0; i < Sh[indexLayer].size(); i++)//neurona i-esima de la capa indexLayer.
						{
							Sh[indexLayer][i] = Sow;
							switch(topology[indexLayer].AF)
							{
									case ActivationFuntion::SIGMOIDEA:
										//std::cout << "sigma : " << NEURONA(lastlayer,i).sigma << "\n";
										Sh[indexLayer][i] *=  Perceptron<T>::sigmoidea_D(NEURONA(indexLayer,i).result);
									break;
									case ActivationFuntion::IDENTITY:
										//So[i] *= T(1);
									break;
									default:
										throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
							}
						}

					}

					if(learning.variable)
					{
						//if(E_prev < E) learning.ratio = learning.ratio * learning.p;
						//else if(E_prev > E) learning.ratio = learning.ratio * learning.r;
						//else learning.ratio = learning.ratio * learning.r;
					}				
					for(Index i = 0; i < So.size(); i++)
					{
						for(Index j = 0; j < std::vector<Layer<T>>::at(lastlayer).at(i).weight.size(); j++)
						{
							WEIGHT(lastlayer,i,j) = WEIGHT(lastlayer,i,j) + ( learning.ratio * So[i] * (*INPUT(lastlayer,i,j)));
						}
					}
					for(Index indexLayer = lastlayer - 1; indexLayer >= 0; indexLayer--)//capa
					{
						for(Index neurona = 0; neurona < LAYER(indexLayer).size(); neurona++)
						{
							for(Index weight = 0; weight < NEURONA(indexLayer,neurona).weight.size(); weight++)
							{
								for(Index input = 0; input < LAYER(0).size(); input++)
								{
									WEIGHT(indexLayer,neurona,weight) = WEIGHT(indexLayer,neurona,weight) + (learning.ratio * Sh[indexLayer][neurona] * (*INPUT(0,input,0)));
								}
							}
						}
					}
				}
				
			}
			return false;
		}*/
		/*bool bp(const std::vector<Data<T>>& datas, Learning<T>& learning, Plotting<T>* plotting)
		{
			Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
			
			if(plotting != NULL)
			{
				plotting->plotter.set_terminal("qt");
				//std::string titleplot = "dEdR";
				//plotByIt->set_title(titleplot);
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotting->plotter.set_label(labelRatio,10,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.mE);
				plotting->plotter.set_label(labeldEdR,10,00.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotting->plotter.set_label(labelCountData,10,0.05);
			}

			//
			T E_prev = 0;
			for(Index indexData = 0; indexData < datas.size(); indexData++)
			{
				//
				T E = 0;
				T Em = 0;
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					spread(datas[indexData].inputs);
					Em = 0;
					for(Index i = 0; i < LAYER(lastlayer).size(); i++)
					{
						Em = datas[indexData].outputs[i] - (*LAYER(lastlayer).get_outputs()[i]);
						Em = std::pow(Em,T(2));
						E += Em;
					}
				}
				E = E / (T(datas.size()) * T(2))  ;//*
				//
				if(plotting != NULL)
				{
					plotting->last++;
					std::vector<T> vecerr(2);
					vecerr[0] = plotting->last;
					vecerr[1] = E;
					plotting->data.push_back(vecerr);
					plotting->plotter.plotting(plotting->data);
				}
				if(learning.mE > E) return true;

				
				
				for(Index it = 0; it < learning.iterations; it++)
				{
					spread(datas[indexData].inputs);
					std::vector<T> So;
					So.resize(std::vector<Layer<T>>::at(lastlayer).get_outputs().size());
					//calculo del error
					for(Index i = 0; i < So.size(); i++)
					{
						So[i] = datas[indexData].outputs[i] - (*std::vector<Layer<T>>::at(lastlayer).get_outputs()[i]);						
						//So[i] = E;
						switch(topology[lastlayer].AF)
						{
								case ActivationFuntion::SIGMOIDEA:
									//std::cout << "sigma : " << NEURONA(lastlayer,i).sigma << "\n";
									So[i] *=  Neurona<T>::sigmoide_D(NEURONA(lastlayer,i).sigma);
								break;
								case ActivationFuntion::IDENTITY:
									//So[i] *= T(1);
								break;
								default:
									throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
						};						
					}
					
					T Sow = 0;
					for(Index i = 0; i < So.size(); i++)
					{
						for(Index j = 0; j < NEURONA(lastlayer,i).weight.size(); j++)
						{
							Sow += So[i] * WEIGHT(lastlayer,i,j);
						}
					}
					
					
					std::vector<std::vector<T>> Sh;
					Sh.resize(std::vector<Layer<T>>::size() - 1);
					for(Index indexLayer = lastlayer - 1; indexLayer > 0; indexLayer--)//capa
					{
						Sh[indexLayer].resize(LAYER(indexLayer).get_outputs().size());
						for(Index i = 0; i < Sh[indexLayer].size(); i++)//neurona i-esima de la capa indexLayer.
						{
							Sh[indexLayer][i] = Sow;
							switch(topology[indexLayer].AF)
							{
									case ActivationFuntion::SIGMOIDEA:
										//std::cout << "sigma : " << NEURONA(lastlayer,i).sigma << "\n";
										Sh[indexLayer][i] *=  Neurona<T>::sigmoide_D(NEURONA(indexLayer,i).sigma);
									break;
									case ActivationFuntion::IDENTITY:
										//So[i] *= T(1);
									break;
									default:
										throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
							}
						}
					}

					//actualizacion de pesos
					if(learning.variable)
					{
						if(E_prev < E) learning.ratio = learning.ratio * learning.p;
						else if(E_prev > E) learning.ratio = learning.ratio * learning.r;
						else learning.ratio = learning.ratio * learning.r;
					}				
					for(Index i = 0; i < So.size(); i++)
					{
						for(Index j = 0; j < std::vector<Layer<T>>::at(lastlayer).at(i).weight.size(); j++)
						{
							WEIGHT(lastlayer,i,j) = WEIGHT(lastlayer,i,j) + ( learning.ratio * So[i] * (*INPUT(lastlayer,i,j)));
						}
					}
					for(Index indexLayer = lastlayer - 1; indexLayer > 0; indexLayer--)//capa
					{
						for(Index neurona = 0; neurona < LAYER(indexLayer).size(); neurona++)
						{
							for(Index weight = 0; weight < NEURONA(indexLayer,neurona).weight.size(); weight++)
							{
								for(Index input = 0; input < LAYER(0).size(); input++)
								{
									WEIGHT(indexLayer,neurona,weight) = WEIGHT(indexLayer,neurona,weight) + (learning.ratio * Sh[indexLayer][neurona] * (*INPUT(0,input,0)));
								}
							}
						}
					}
				}				  
				E_prev = E;
				nextData:
					;
			}
			
			return false;
		}*/
		bool bp(const std::vector<Data<T>>& datas, const Learning<T>& learning, Plotting<T>* plotting)
		{
			Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
			outs = &std::vector<Layer<T>>::at(lastlayer).get_outputs();
			std::ofstream filePlotting;
			if(plotting != NULL)
			{
				filePlotting.open(plotting->filename,std::ios::app);
				plotting->plotter.set_terminal("qt");
				//std::string titleplot = "dEdR";
				//plotByIt->set_title(titleplot);
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotting->plotter.set_label(labelRatio,10,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.mE);
				plotting->plotter.set_label(labeldEdR,10,0.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotting->plotter.set_label(labelCountData,10,0.05);
			}
						
			//solo para la ultima capa
			std::vector<T> dEdR,dRdZ;
			dEdR.resize(LAYER(lastlayer).size());
			dRdZ.resize(LAYER(lastlayer).size());
			
			T mdEdR_mean,mdEdR_set;
			T count = 0;
			//std::cout << "Step 1.0\n";
			for(Index it = 0; it < learning.iterations; it++)
			{
				mdEdR_set = 0;
				count = 0;
				//std::cout << "Step 1.0.1.0\n";
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					//std::cout << "Step 1.0.1.0.1.0\n";
					spread(datas[indexData].inputs);
					for(Index out = 0; out < dEdR.size(); out++)
					{
						dEdR[out] = datas[indexData].outputs[out] - (*std::vector<Layer<T>>::at(lastlayer).get_outputs()[out]);	
					}
					//std::cout << "Step 1.0.1.0.1.1\n";
					mdEdR_mean = 0;
					for(Index i = 0; i < dEdR.size(); i++)
					{
						mdEdR_mean += std::abs(dEdR[i]);
					}
					if(dEdR.size() > 1) mdEdR_mean /= T(dEdR.size());	
					if(mdEdR_mean < learning.mE) continue;
					mdEdR_set += mdEdR_mean;
					
					//std::cout << "Step 1.0.1.0.1.2\n";
					//derivada de la activacion respecto a la suman ponderada
					for(Index i = 0; i < dRdZ.size(); i++)
					{
						switch(topology[lastlayer].AF)
						{
							case ActivationFuntion::SIGMOIDEA:
								dRdZ[i] =  Perceptron<T>::sigmoidea_D(std::vector<Layer<T>>::at(lastlayer).at(i).result);
							break;
							case ActivationFuntion::IDENTITY:
								dRdZ[i] = T(1);
							break;
							default:
								throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
						};
					}

					//std::cout << "Step 1.0.1.0.1.3\n";
					T dEdW = 0;
					for(Index out = 0; out < dEdR.size(); out++)
					{
						dEdW += dEdR[out] * dRdZ[out];
					}	
					//std::cout << "Step 1.0.1.0.1.4.0\n";
					for(Index indexLayer = lastlayer; indexLayer >= 0; indexLayer--)
					{
						//std::cout << "Step 1.0.1.0.1.4.0.1\n";
						Index neurona = max(LAYER(indexLayer));	
						Index input = max(NEURONA(indexLayer,neurona));
						//std::cout << "Step 1.0.1.0.1.4.0.2\n";
						dEdW *= *INPUT(indexLayer,neurona,input);//weight es el mismo indice de la input

						//std::cout << "Step 1.0.1.0.1.4.0.3\n";
						WEIGHT(indexLayer,neurona,input) = WEIGHT(indexLayer,neurona,input) - (learning.ratio * dEdW);											
					}
					//std::cout << "Step 1.0.1.0.1.5\n";
				}
				mdEdR_set/=count;
				if(mdEdR_set < learning.mE) 
				{
					filePlotting.flush();
					filePlotting.close();
					return true;
				}
				if(plotting != NULL)
				{
					plotting->last++;
					oct::math::Plotter::save(filePlotting,plotting->last,mdEdR_set);
					filePlotting.flush();
					plotting->plotter.plottingFile2D(plotting->filename);
				}
			}
			
			filePlotting.flush();
			filePlotting.close();
			return false;
		}
/*
				if(plotting != NULL)
				{
					plotting->last++;
					std::vector<T> vecerr(2);
					vecerr[0] = plotting->last;
					vecerr[1] = mdEdR_mean;
					plotting->data.push_back(vecerr);
					plotting->plotter.plotting(plotting->data);
				}
*/

		Index max(const Layer<T>& layer)
		{
			Index maxIndex = 0;
			T maxFound = *layer.get_outputs()[maxIndex];
			for(Index i = 0; i < layer.size(); i++)
			{
				if(*layer.get_outputs()[i] > maxFound) 
				{
					maxFound = *layer.get_outputs()[i];
					maxIndex = i;
				}
			}

			return maxIndex;
		}
		Index max(const Perceptron<T>& perceptron)
		{
			Index maxIndex = 0;
			T maxFound = perceptron.weight[maxIndex];
			for(Index i = 0; i < perceptron.weight.size(); i++)
			{
				if(perceptron.weight[i] > maxFound) 
				{
					maxFound = perceptron.weight[i];
					maxIndex = i;
				}
			}

			return maxIndex;
		}
		void display(const Data<T>& data,oct::math::Plotter* plotGraph, const Learning<T>& learning)
		{
			plotGraph->set_terminal("qt");
			//std::string titleplot = "funcion";
			//plotGraph->set_title(titleplot);
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
	};
}

#endif