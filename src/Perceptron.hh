
#ifndef OCTETOS_NEURONAL_PERCEPTRON_HH
#define OCTETOS_NEURONAL_PERCEPTRON_HH

#include <vector>
#include <octetos/math/Vector.hh>

namespace oct::neu
{
	typedef unsigned short Index;

	
	template<typename T> T sigmoide(T v)
	{
		return 1.0/(1.0 + exp(-1.0 * v));
	}
	template<typename T> T sigmoide_D(T v)
	{
		return v*(1.0-v);
	}


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
			for(unsigned short i = 0; i < weight.size(); i++)
			{
				weight[i] = 0.0;
			}
		}
		
		std::vector<T*>& get_inputs()
		{
			return inputs;
		}
		oct::math::Vector<T>& get_weight()
		{
			return weight;
		}
		T& get_out()
		{
			return out;
		}

		void set_inputs(const oct::math::Vector<T*>& v)
		{
			if(inputs.size() != v.size()) throw octetos::core::Exception("Los tamanos de los vectores involucrados no son iguales",__FILE__,__LINE__);
			for(unsigned short i = 0; i < v.size(); i++)
			{
				inputs[i] = v[i];
			}
		}

		T spread(T (*F)(T))
		{
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 1\n";
			T s = sigma();
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 2\n";
			out =  F(s);
			//std::cout << "\tvoid datatype Perceptron::spread(datatype (*F)(datatype)) step 3\n";
			return out;
		}
		void resize(unsigned short ins)
		{
			inputs.resize(ins);
			weight.resize(ins);
		}
		void gd(T )
		{
			
		}
		
	private:
		std::vector<T*> inputs;
		math::Vector<T> weight;
		T out;

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