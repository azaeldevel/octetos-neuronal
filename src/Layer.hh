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
		void gd(unsigned short maxit, T ratio, Layer& prevL, const std::vector<T>& expected)
		{
			//calcular el error medio
			T me = 0;
			for(Index i = 0; i < outputs.size(); i++)
			{
				me += std::pow(expected[i] - *outputs[i],T(2.0));
			}
			me /= 2.0;

			//
			T dFdA = 0;					
		}
		

	private:
		T (*FA)(T);
		T (*D)(T);

	private:
		std::vector<T*> outputs;
		ActivationFuntion AF;
	};


}


#endif