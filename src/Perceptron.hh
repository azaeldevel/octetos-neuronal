
#ifndef OCTETOS_NEURONAL_PERCEPTRON_HH
#define OCTETOS_NEURONAL_PERCEPTRON_HH

#include "neuronal.hh"

namespace oct::neu
{
	
	
	enum ActivationFuntion
	{
		NONE,
		SIGMOIDEA,
		TANH,
		RELU,
		SCALON
	};

	template<typename T> class Perceptron
	{
	public:
		Perceptron()
		{		
		}
		Perceptron(unsigned short ins)
		{
			set(ins);
		}
		void set(unsigned short ins)
		{
			inputs.resize(ins);
			weight.resize(ins);
		}
		
		std::vector<T*>& get_inputs()
		{
			return inputs;
		}
		std::vector<T>& get_weight()
		{
			return weight;
		}
		T& get_out()
		{
			return out;
		}

		void set_inputs(const std::vector<T*>& v)
		{
			if(inputs.size() != v.size()) throw oct::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
			for(unsigned short i = 0; i < v.size(); i++)
			{
				inputs[i] = v[i];
			}
		}

		T spread(ActivationFuntion af)
		{
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 1\n";
			T s = sigma();
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 2\n";
			switch(af)
			{
				case ActivationFuntion::SIGMOIDEA:
					out =  sigmoide(s);
				break;
				default:
					throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
			};
			
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 3\n";
			return out;
		}
		void resize(unsigned short ins)
		{
			inputs.resize(ins);
			weight.resize(ins);
		}
		void gd(T expected,T ratio)
		{
						
		}
		
		static T sigmoide(T v)
		{
			return T(1)/(T(1) + exp(T(-1) * v));
		}
		static T sigmoide_D(T v)
		{
			return v * (T(1) - v);
		}

	private:
		std::vector<T*> inputs;
		std::vector<T> weight;
		T out;
		T umbral;
		T sesgo;

	private:
		
		T sigma()
		{
			//std::cout << "\tdatatype Perceptron::sigma step 1\n";
			T val = 0;
			//std::cout << "\tdatatype Perceptron::sigma step 2\n";
			if(inputs.size() != weight.size()) throw octetos::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
			//std::cout << "\tdatatype Perceptron::sigma step 3\n";
			
			for(unsigned short i = 0; i < inputs.size(); i++)
			{
				//std::cout << "\tdatatype Perceptron::sigma step 3.1 - " << i << "\n";
				val += (*inputs[i]) * weight[i];
			}
			
			//std::cout << "\tdatatype Perceptron::sigma step 4\n";
			return val;
		}
	};

}

#endif