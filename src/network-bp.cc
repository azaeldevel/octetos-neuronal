
#include <octetos/core/Error.hh>
#include <cmath>
#include <iostream>


#include "network.hh"

namespace oct::neu
{
	

	
	
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
		bool Network::bp(const std::vector<Data<DATATYPE>>& datas, const Learning<DATATYPE>& learning, Plotting<DATATYPE>* plotting)
		{
			Index lastlayer = std::vector<Layer<DATATYPE>>::size() - 1;//optener la ultima capa
			outs = &std::vector<Layer<DATATYPE>>::at(lastlayer).get_outputs();
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
			std::vector<DATATYPE> dEdR,dRdZ;
			dEdR.resize(LAYER(lastlayer).size());
			dRdZ.resize(LAYER(lastlayer).size());
			
			DATATYPE mdEdR_mean,mdEdR_set;
			DATATYPE count = 0;
			for(Index it = 0; it < learning.iterations; it++)
			{
				mdEdR_set = 0;
				count = 0;
				for(Index indexData = 0; indexData < datas.size(); indexData++)
				{
					spread(datas[indexData].inputs);
					for(Index out = 0; out < dEdR.size(); out++)//calcula el error en cada salida
					{
						dEdR[out] = datas[indexData].outputs[out] - (*std::vector<Layer<DATATYPE>>::at(lastlayer).get_outputs()[out]);	
					}
					mdEdR_mean = 0;
					
					//calcula el promiedio de erro en todas las salidas
					if(dEdR.size() > 1)
					{
						for(Index i = 0; i < dEdR.size(); i++)
						{
							mdEdR_mean += std::abs(dEdR[i]);
						}
						mdEdR_mean /= DATATYPE(dEdR.size());
					}	
					else
					{
						mdEdR_mean = std::abs(dEdR[0]);
					}
					if(mdEdR_mean < learning.mE) continue;//siguiente dato
					mdEdR_set += mdEdR_mean;
					count++;//para plotting
					
					//derivada de la activacion respecto a la suman ponderada
					for(Index i = 0; i < dRdZ.size(); i++)
					{
						switch(topology[lastlayer].AF)
						{
							case ActivationFuntion::SIGMOIDEA:
								dRdZ[i] =  Perceptron<DATATYPE>::sigmoidea_D(std::vector<Layer<DATATYPE>>::at(lastlayer).at(i).result);
							break;
							case ActivationFuntion::IDENTITY:
								dRdZ[i] = DATATYPE(1);
							break;
							default:
								throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
						};
					}

					DATATYPE dEdZ = 0;
					for(Index out = 0; out < dEdR.size(); out++)
					{
						dEdZ += dEdR[out] * dRdZ[out];
					}	
					for(int indexLayer = lastlayer; indexLayer >= 0; indexLayer--)
					{
						Index neurona = max(LAYER(indexLayer));	
						//Index weight = max_weight(NEURONA(indexLayer,neurona));
						for(Index i = 0; i < NEURONA(indexLayer,neurona).inputs.size(); i++)
						{
							WEIGHT(indexLayer,neurona,i) = WEIGHT(indexLayer,neurona,i) + (learning.ratio * dEdZ * (*INPUT(indexLayer,neurona,i)));		
						}									
					}
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
					//std::cout << "(" << plotting->last << "," << mdEdR_set << ")\n";
					filePlotting.flush();
					plotting->plotter.plottingFile2D(plotting->filename);
				}
			}
			
			filePlotting.flush();
			filePlotting.close();
			return false;
		}
}
