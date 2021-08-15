#ifndef OCTETOS_NEURONAL_LAYER_HH
#define OCTETOS_NEURONAL_LAYER_HH

#include <Perceptron.hh>
#include <list>

namespace oct::neu
{

	template<typename T> class Layer : public std::vector<Perceptron<T>>
	{
	public:
		Layer()
		{		
		}
		/**
		*\param inputsP Entradas por perceptron
		*\param countP cantidad de perseptornes en la capa
		*\param FA funcion de activavion
		*/
		Layer(unsigned short inputsP, unsigned short countP, ActivationFuntion af)
		{
			set(inputsP,countP,AF);
		}
		void set(unsigned short inputsP, unsigned short countP, ActivationFuntion af)
		{
			if(std::vector<Perceptron<T>>::size() < countP) std::vector<Perceptron<T>>::resize(countP);
			
			AF = af;
			for(Perceptron<T>& p : *this)
			{
				p.set(inputsP);
			}
			
			outputs.resize(std::vector<Perceptron<T>>::size());
			//std::cout << "size = " << std::vector<Perceptron<T>>::size() << "\n";
			for(unsigned short i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{//asigna la salida de cada perceptor al vetor de salida
				outputs[i] = &std::vector<Perceptron<T>>::at(i).get_out(); 
			}
			dEdR.resize(outputs.size());	
			dRdZ.resize(outputs.size());
			dZdW.resize(outputs.size());
			dEdW.resize(outputs.size());
			dEdZ.resize(outputs.size());
			gradientDescent.resize(outputs.size());
			dRdW.resize(outputs.size());
			En.resize(outputs.size());
			En1.resize(outputs.size());
		}

		std::vector<T*>& get_outputs()
		{
			return outputs;
		}

		static void print(const std::vector<T>& v)
		{
			std::cout << "(";
			for(unsigned short i = 0; i < v.size(); i++)
			{
				std::cout << v[i];
				if(i < v.size() - 1) std::cout << ",";
			}
			std::cout << ")";
		}
		static void print(const std::vector<T*>& v)
		{
			std::cout << "(";
			for(unsigned short i = 0; i < v.size(); i++)
			{
				std::cout << *v[i];
				if(i < v.size() - 1) std::cout << ",";
			}
			std::cout << ")";
		}

		void spread()
		{
			//std::cout << "\tvoid Layer::spread(): step 1 \n";
			for(unsigned short i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{
				//std::cout << "\tvoid Layer::spread(): step 1.1\n";
				std::vector<Perceptron<T>>::at(i).spread(AF);			
			}
			//std::cout << "\tvoid Layer::spread(): step 2 \n";
		}
		/**
		*\brief 
		*/
		void gd(unsigned short maxit, T ratio, Layer& prevL, const Data<T>& expected)
		{
			//std::cout << "\tvoid Layer::gd(..) : step 1\n";
			for(Index it = 0; it < maxit; it++)
			{
				//std::cout << "Iteracion : " << it << "\n";
				for(Index i = 0; i < En.size(); i++)
				{
					En[i] = expected.outputs[i] - *outputs[i];
				}
				//derivada partcial respecto a la funcion de activacion			
				for(Index i = 0; i < dEdR.size(); i++)
				{
					dEdR[i] = *outputs[i] - expected.outputs[i];
				}
			
				//std::cout << "\tvoid Layer::gd(..) : step 2\n";
				//derivada de la activacion respecto a la suman ponderada
				for(Index i = 0; i < dRdZ.size(); i++)
				{
					switch(AF)
					{
						case ActivationFuntion::SIGMOIDEA:
							//std::cout << "sigma : " << std::vector<Perceptron<T>>::at(i).get_sigma() << "\n";
							dRdZ[i] =  Perceptron<T>::sigmoide_D(std::vector<Perceptron<T>>::at(i).get_sigma());
						break;
						case ActivationFuntion::IDENTITY:
							dRdZ[i] =  Perceptron<T>::identity_D(std::vector<Perceptron<T>>::at(i).get_sigma());
						break;
						default:
							throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
					};
				}

				//std::cout << "\tvoid Layer::gd(..) : step 3\n";
				//la derivada parcial de  la suman ponderada respecto de los pesos
				for(Index i = 0; i < dZdW.size(); i++)
				{
					//if(dZdW.size() != std::vector<Perceptron<T>>::at(i).get_inputs().size()) dZdW.resize(std::vector<Perceptron<T>>::at(i).get_inputs().size());
					dZdW[i] = std::vector<Perceptron<T>>::at(i).derivade();
				}

				//std::cout << "\tvoid Layer::gd(..) : step 4\n";
				//derivada parcial del error repecto de los pesos
				for(Index i = 0; i < dEdW.size(); i++)
				{
					dEdW[i] =  dEdR[i] * dRdZ[i] * dZdW[i];
				}

				//std::cout << "\tvoid Layer::gd(..) : step 5\n";
				//error imputado
				for(Index i = 0; i < dEdZ.size(); i++)
				{
					dEdZ[i] =  dEdR[i] * dRdZ[i];
				}
				
				//std::cout << "\tvoid Layer::gd(..) : step 6\n";
				for(Index i = 0; i < gradientDescent.size(); i++)
				{
					gradientDescent[i] = dEdW[i] * T(-1.0);
				}
					
				//std::cout << "\tvoid Layer::gd(..) : step 7\n";
				for(Index i = 0; i < dRdW.size(); i++)
				{
					dRdW[i] = dRdZ[i] * dZdW[i];
				}

				//std::cout << "\tvoid Layer::gd(..) : step 8\n";
				Index highIndex = max(dEdZ);//neurona de mayor responsabilidad
				//std::cout << "\tvoid Layer::gd(..) : step 9\n";
				wlakingDown(highIndex,ratio);
				//std::cout << "\tvoid Layer::gd(..) : step 10\n";
				std::cout << "dEdR = ";
				Layer<T>::print(dEdR);
				std::cout << "\n";
				std::cout << "dRdZ = ";
				Layer<T>::print(dRdZ);
				std::cout << "\n";
				//std::cout << "dEdW = ";
				//Layer<T>::print(dEdW);
				//std::cout << "\n";
				//Layer<T>::print(Layer<T>::at(highIndex).get_inputs());
				//std::cout << "\n";
				std::cout << "weight : ";
				Layer<T>::print(Layer<T>::at(highIndex).get_weight());
				std::cout << "\n";
				std::cout << "outputs : ";
				Layer<T>::print(outputs);
				std::cout << "\n";
				
				spread();
				for(Index i = 0; i < En1.size(); i++)
				{
					En1[i] = expected.outputs[i] - *outputs[i];
				}
			}
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
		void wlakingDown(Index n, T ratio)
		{
			std::vector<T>& ws = std::vector<Perceptron<T>>::at(n).get_weight();
			for(Index i = 0; i < ws.size(); i++)
			{
				ws[i] = ws[i]  - (dEdW[n] * ratio);
			}
		}
		

	private:

	private:
		std::vector<T*> outputs;
		ActivationFuntion AF;
		std::vector<T> dEdR,dRdZ,dZdW,dEdW,dEdZ,dRdW,gradientDescent,En,En1;
	};


}


#endif