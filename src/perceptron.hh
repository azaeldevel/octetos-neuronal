
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
		SCALON,
		IDENTITY
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
			for(Index i = 0; i < weight.size(); i++)
			{
				double randS = core::randNumber();
				if(randS > 0.5) weight[i] = core::randNumber();
				else weight[i] = core::randNumber() * -1.0;
			}
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
		T& get_sigma()
		{
			return sigma;
		}

		void set_inputs(const std::vector<T*>& v)
		{
			if(inputs.size() != v.size()) throw oct::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
			for(unsigned short i = 0; i < v.size(); i++)
			{
				inputs[i] = v[i];
			}
		}

		T spread(ActivationFuntion AF)
		{
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 1\n";
			sigma = sum();
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 2\n";
			switch(AF)
			{
				case ActivationFuntion::SIGMOIDEA:
					out =  sigmoide(sigma);
				break;
				case ActivationFuntion::IDENTITY:
					out =  sigma;
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
		T sum()
		{
			//std::cout << "\tdatatype Perceptron::sigma step 1\n";
			T val = 0;
			//std::cout << "\tdatatype Perceptron::sigma step 2\n";
			if(inputs.size() != weight.size()) throw oct::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
			//std::cout << "\tdatatype Perceptron::sigma step 3\n";
			
			for(unsigned short i = 0; i < inputs.size(); i++)
			{
				//std::cout << "\tdatatype Perceptron::sigma step 3.1 - " << i << "\n";
				val += (*inputs[i]) * weight[i];
			}
			
			//std::cout << "\tdatatype Perceptron::sigma step 4\n";
			return val;
		}
		/**
		*\brief Gradient Stteping
		*/
		void gd(T ratio, T dEdW)
		{
			//std::cout << "weight : ";
			//print(weight);
			/*if(dEdW > 0)
			{
				for(Index i = 0; i < weight.size(); i++)
				{
					weight[i] = weight[i]  - (dEdW * ratio);
				}
			}
			else if(dEdW < 0)
			{
				for(Index i = 0; i < weight.size(); i++)
				{
					weight[i] = weight[i]  + (dEdW * ratio);
				}
			}*/
			for(Index i = 0; i < weight.size(); i++)
			{
				weight[i] = weight[i]  - (dEdW * ratio);
			}

			//std::cout << " -> ";
			//print(weight);
			//std::cout << "\n";
		}

		static T sigmoide(T v)
		{
			return T(1)/(T(1) + exp(T(-1) * v));
		}
		static T sigmoide_D(T v)
		{
			return sigmoide(v) * (T(1) - sigmoide(v));
		}
		static T identity(T v)
		{
			return v;
		}
		static T identity_D(T v)
		{
			return T(1);
		}

	private:
		std::vector<T*> inputs;
		std::vector<T> weight;
		T out;
		//T umbral;
		//T sesgo;
		T sigma;

	private:
		
		
	};

}

#endif