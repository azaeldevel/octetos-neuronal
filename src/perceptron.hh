
#ifndef OCTETOS_NEURONAL_PERCEPTRON_HH
#define OCTETOS_NEURONAL_PERCEPTRON_HH

#include "neuronal.hh"

namespace oct::neu
{
	
	
	enum ActivationFuntion
	{
		NONE,
		SIGMOID,
		TANH,
		RELU,
		SCALON,
		IDENTITY
	};

	template<typename T> struct Perceptron
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
				if(randS > 0.5)weight[i] = core::randNumber(0.0,0.5);
				else weight[i] = core::randNumber(0.0,0.5) * -1.0;
			}
			//double randS = core::randNumber();
			//if(randS > 0.5) theta = core::randNumber(0.0,0.5);
			//else theta = core::randNumber(0.0,0.5) * -1.0;
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
			return result;
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
			result = sigma();

			switch(AF)
			{
				case ActivationFuntion::SIGMOID:
					out = sigmoid(result);
				break;
				case ActivationFuntion::IDENTITY:
					out = result;
				break;
				case ActivationFuntion::SCALON:
					out = scalon(result);
				break;
				case ActivationFuntion::RELU:
					out = relu(result);
				break;
				default:
					throw oct::core::Exception("Funcion de activacion desconocida",__FILE__,__LINE__);
			};
			
			return out;
		}
		
		void resize(unsigned short ins)
		{
			inputs.resize(ins);
			weight.resize(ins);
		}
		T sigma()
		{
			T val = 0;

			if(inputs.size() != weight.size()) throw oct::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
			
			for(unsigned short i = 0; i < inputs.size(); i++)
			{
				val += (*inputs[i]) * weight[i];
			}
			//val += theta;

			return val;
		}

		static T sigmoid(T v)
		{
			return T(1)/(T(1) + exp(-v));
		}
		static T sigmoid_D(T v)
		{
			return v * (T(1) - v);
		}
		static T identity(T v)
		{
			return v;
		}
		static T identity_D(T v)
		{
			return T(1);
		}
		static T scalon(T v)
		{
			if(v > 0.5) return T(1);
			else  return T(0);
		}
		static T relu(T v)
		{
			if(v < 0) return T(0);
			else  return T(v);
		}
		static T relu_D(T v)
		{
			if(v < 0) return T(0);
			else  return T(1);
		}

		std::vector<T*> inputs;
		std::vector<T> weight;
		T out;
		//T umbral;
		//T sesgo;
		T result;
		//T theta;
		
	};

}

#endif