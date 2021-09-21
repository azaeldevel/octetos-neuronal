
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
		*/
		void bp(const std::vector<Data<T>>& datas, Learning<T>& learning, Plotting<T>* plotting)
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
				if(learning.mE > E) return;

				
				
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
					/*std::cout << "\tExpected : " << datas[indexData].outputs[0] << "\n";
					std::cout << "\tPredicted : " << (*std::vector<Layer<T>>::at(lastlayer).get_outputs()[0]) << "\n";
					std::cout << "\tSo :";
					print(So);
					std::cout << "\n";*/
					std::vector<std::vector<T>> Sh;
					Sh.resize(std::vector<Layer<T>>::size() - 1);
					for(Index indexLayer = lastlayer - 1; indexLayer > 0; indexLayer--)//capa
					{
						Sh[indexLayer].resize(LAYER(indexLayer).get_outputs().size());
						for(Index i = 0; i < Sh[indexLayer].size(); i++)//neurona i-esima de la capa indexLayer.
						{
							Sh[indexLayer][i] = 0;
							for(Index j = 0; j < So.size(); j++)
							{
								for(Index k = 0; k < NEURONA(lastlayer,j).weight.size(); k++)
								{
									Sh[indexLayer][i] += So[j] * NEURONA(lastlayer,j).weight[k];
									switch(topology[lastlayer].AF)
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
									};
								}
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
								for(Index input = 0; input < NEURONA(0,input).inputs.size(); input++)
								{
									WEIGHT(indexLayer,neurona,weight) = WEIGHT(indexLayer,neurona,weight) + (learning.ratio * Sh[indexLayer][neurona] * (*INPUT(0,input,0)));
								}
							}
						}
						for(Index i = 0; i < Sh.size(); i++)
						{
							switch(topology[indexLayer].AF)
							{
									case ActivationFuntion::SIGMOIDEA:
										//std::cout << "sigma : " << std::vector<Perceptron<T>>::at(i).get_sigma() << "\n";
										Sh[indexLayer][i] *=  Neurona<T>::sigmoide_D(NEURONA(indexLayer,i).get_sigma());
									break;
									case ActivationFuntion::IDENTITY:
										//So[i] *= T(1);
									break;
									default:
										throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
							};
						}
					}
				}				  
				E_prev = E;
			}
			if(plotting != NULL)
			{
				std::string output = "output.svg";
				//output += std::to_string(indexData) + ".svg";
				plotting->plotter.set_terminal("svg");
				plotting->plotter.set_output(output);
				plotting->plotter.plotting(plotting->data);
			}
		}
		/*void bp(const std::vector<Data<T>>& datas, const Learning<T>& learning, Plotting<T>* plotting)
		{
			//std::cout << "\tvoid Network::bp(..) : step 1\n";
			Index lastlayer = std::vector<Layer<T>>::size() - 1;//optener la ultima capa
			outs = &std::vector<Layer<T>>::at(lastlayer).get_outputs();
			//std::cout << "\tvoid Network::bp(..) : step 2\n";
			//std::list<std::vector<T>> errDataPlot;
			//std::list<std::vector<T>> fDataPlot;
			if(plotting != NULL)
			{
				plotting->plotter.set_terminal("qt");
				//std::string titleplot = "dEdR";
				//plotByIt->set_title(titleplot);
				std::string labelRatio = "ratio = ";
				labelRatio += std::to_string(learning.ratio);
				plotting->plotter.set_label(labelRatio,10,0.15);
				std::string labeldEdR = "max dEdR = ";
				labeldEdR += std::to_string(learning.dEdR);
				plotting->plotter.set_label(labeldEdR,10,0.10);
				std::string labelCountData = "Data = ";
				labelCountData += std::to_string(datas.size());
				plotting->plotter.set_label(labelCountData,10,0.05);
			}
						
			std::vector<T> dEdR,dRdZ,dEdZ,dZdW,dEdW;
			//std::vector<std::vector<T>> dRdZ_history;
			//dEdR.resize(std::vector<Layer<T>>::at(lastlayer).get_outputs().size());
			//dRdZ_history.resize(std::vector<Layer<T>>::size());
			dEdR.resize(std::vector<Layer<T>>::at(lastlayer).get_outputs().size());

			for(Index it = 0; it < learning.iterations; it++)
			{
				//std::cout << "\tIteracion : " << it << "\n";
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					spread(datas[indexData].inputs);
					for(Index i = 0; i < dEdR.size(); i++)
					{
						dEdR[i] += (*std::vector<Layer<T>>::at(lastlayer).get_outputs()[i]) - datas[indexData].outputs[i];						
					}
				}
				T mdEdR_mean = 0;
				for(Index i = 0; i < dEdR.size(); i++)
				{
					dEdR[i] /= datas.size();
					mdEdR_mean += std::abs(dEdR[i]);
				}
				if(dEdR.size() > 1) mdEdR_mean /= T(dEdR.size());	
				if(mdEdR_mean < learning.dEdR) break;

				//for(Index indexData = 0; indexData < datas.size(); indexData++)
				//{
					//std::cout << "\tvoid Network::bp(..) : step 2.1\n";
					//std::cout << "\t>>Data :";
					//std::cout << "\t";
					//Layer<T>::print(datas[indexData].inputs);
					//std::cout << "\tprev :";
					//Layer<T>::print(*outs);
					//std::cout << "\n";
					
					//

					for(Index indexLayer = lastlayer; indexLayer > 0; indexLayer--)
					{
						//std::cout << ">Capa :" << indexLayer << "\n";
						//std::cout << "Dato : " << indexData << "\n";
						//std::cout << "Media dEdR : " << mdEdR_Data << "\n";
						//print(std::vector<Layer<T>>::at(lastlayer).get_outputs());
						//std::cout << " | ";	
						//print(datas[indexData].outputs);				
						//std::cout << "\n";
						//std::cout << "\tvoid Network::bp(..) : step 2.1.1\n";
						dEdZ.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						dZdW.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						dRdZ.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
						dEdW.resize(std::vector<Layer<T>>::at(indexLayer).get_outputs().size());
			
						//derivada de la activacion respecto a la suman ponderada
						for(Index i = 0; i < dRdZ.size(); i++)
						{
							switch(topology[indexLayer].AF)
							{
								case ActivationFuntion::SIGMOIDEA:
									//std::cout << "sigma : " << std::vector<Perceptron<T>>::at(i).get_sigma() << "\n";
									dRdZ[i] =  Neurona<T>::sigmoide_D(std::vector<Layer<T>>::at(indexLayer).at(i).get_sigma());
								break;
								case ActivationFuntion::IDENTITY:
									dRdZ[i] = T(1);
								break;
								default:
									throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
							};
						}

						//la derivada parcial de  la suman ponderada respecto de los pesos
						for(Index i = 0; i < dZdW.size(); i++)
						{
							dZdW[i] = 0;
							for(Index j = 0; j < std::vector<Layer<T>>::at(indexLayer).at(i).get_inputs().size(); j++)
							{
								dZdW[i] = dZdW[i] + *std::vector<Layer<T>>::at(indexLayer).at(i).get_inputs()[j];
							}
						}
						
						//std::cout << "\tvoid Network::bp(..) : step 2.1.3\n";
						//error imputado
						for(Index i = 0; i < dEdZ.size(); i++)
						{
							dEdZ[i] =  dEdR[i] * dRdZ[i];
						}
						
						//std::cout << "\tvoid Network::bp(..) : step 2.1.4\n";
						//derivada parcial del error repecto de los pesos
						for(Index i = 0; i < dZdW.size(); i++)
						{
							dEdW[i] =  dEdZ[i] * dZdW[i];
						}
							
						//std::cout << "\tvoid Network::bp(..) : step 2.1.5\n";	
						
						//std::cout << "dEdR = ";
						//print(dEdR);
						//std::cout << "\n";
						//std::cout << "dRdZ = ";
						//print(dRdZ);
						//std::cout << "\n";
						//std::cout << "dZdW = ";
						//print(dZdW);
						//std::cout << "\n";
						//std::cout << "dEdW = ";
						//print(dEdW);
						//std::cout << "\n";
						//std::cout << "dEdZ = ";
						//print(dEdZ);
						//std::cout << "\n";
						//Layer<T>::print(Layer<T>::at(highIndex).get_inputs());
						//std::cout << "\n";
						//std::cout << "\t\t\tweight : ";
						//Layer<T>::print(Layer<T>::at(indexLayer).get_weight());
						//std::cout << "\n";
						//std::cout << "outputs : ";
						//Layer<T>::print(outputs);
						//std::cout << "\n";
						//std::cout << "\tvoid Network::bp(..) : step 2.1.6\n";
						Index highIndex = max(dEdZ);
						std::vector<Layer<T>>::at(indexLayer).at(highIndex).gd(learning.ratio,dEdW[highIndex]);
						//std::vector<Layer<T>>::at(indexLayer).spread();	
						//std::cout << "\tvoid Network::bp(..) : step 2.1.8\n";
					}
					//std::cout << "\tpost :";
					//Layer<T>::print(*outs);
					//std::cout << " - ";	
					//Layer<T>::print(datas[indexData].outputs);
					//std::cout << "\n";
				//}
				//mdEdR_set /= T(datas.size());
				if(plotting != NULL)
				{
					plotting->last++;
					std::vector<T> vecerr(2);
					vecerr[0] = plotting->last;
					vecerr[1] = mdEdR_mean;
					plotting->data.push_back(vecerr);
					plotting->plotter.plotting(plotting->data);
				}
			}
			if(plotting != NULL)
			{
				std::string output = "output.svg";
				//output += std::to_string(indexData) + ".svg";
				plotting->plotter.set_terminal("svg");
				plotting->plotter.set_output(output);
				plotting->plotter.plotting(plotting->data);
			}
			//std::cout << "\tvoid Network::bp(..) : step 3\n";
		}*/

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
		std::list<Progress> progress;
	};
}

#endif