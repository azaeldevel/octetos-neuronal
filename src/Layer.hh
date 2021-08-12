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
		Layer(unsigned short inputsP, unsigned short countP, T (*fa)(T)) : std::vector<Perceptron<T>>(countP),FA(fa)
		{
			set(inputsP,countP,fa);
		}
		void set(unsigned short inputsP, unsigned short countP, T (*fa)(T))
		{
			if(std::vector<Perceptron<T>>::size() < countP) std::vector<Perceptron<T>>::resize(countP);
			
			FA = fa;
			for(Perceptron<T>& p : *this)
			{
				p.set(inputsP);
			}
			gradient.resize(countP);
			gradient_unit.resize(countP);
			//gradient_descent.resize(countP);
		}

		oct::math::Vector<T>& get_gradient()
		{
			return gradient;
		}
		oct::math::Vector<T>& get_gradient_unit()
		{
			return gradient_unit;
		}
		oct::math::Vector<T*>& get_outputs()
		{
			return outputs;
		}

		static void print(const oct::math::Vector<T>& v)
		{
			std::cout << "(";
			for(unsigned short i = 0; i < v.size(); i++)
			{
				std::cout << v[i];
				if(i < v.size() - 1) std::cout << ",";
			}
			std::cout << ")";
		}
		static void print(const oct::math::Vector<T*>& v)
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
				std::vector<Perceptron<T>>::at(i).spread(FA);			
			}	
			//std::cout << "\tvoid Layer::spread(): step 2\n";
			gradient.resize(std::vector<Perceptron<T>>::size());
			for(unsigned short i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{
				gradient[i] = sigmoide_dev(std::vector<Perceptron<T>>::at(i).get_out());
			}
			//calculando vector unitario
			T leght = 0;
			for(unsigned short i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{
				leght += std::pow(gradient[i],2.0);//suma de cuadrados
			}
			leght = std::sqrt(leght);
			gradient_unit.resize(std::vector<Perceptron<T>>::size());
			for(unsigned short i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{
				gradient_unit[i] = gradient[i]/leght;
			}
			/*gradient_descent.resize(size());
			for(unsigned short i = 0; i < size(); i++)
			{
				gradient_descent[i] = -1.0 * gradient_unit[i];
			}*/
			
			//save outs
			outputs.clear();
			outputs.resize(std::vector<Perceptron<T>>::size());
			//std::cout << "size = " << size() << "\n";
			for(unsigned short i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{
				outputs[i] = &std::vector<Perceptron<T>>::at(i).get_out(); 
			}
		}
		void minimize(unsigned short maxit, T ratio,oct::math::Vector<T>& out, Layer& prevL)
		{
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 1\n";
			if(outputs.size() != gradient_unit.size()) throw octetos::core::Exception("Los vectores son de diferente tamano",__FILE__,__LINE__);
			
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 2\n";
			oct::math::Vector<datatype> newdat(outputs.size());
			std::list<oct::math::Vector<datatype>> options;
			bool running = true;
			unsigned short countit = 0;
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 3\n";
			do
			{
				for(unsigned short i = 0; i < gradient_unit.size(); i++)
				{
					newdat[i] = *outputs[i] - (gradient_unit[i] * ratio * datatype(maxit));
				}	
				//options.push_back(newdat);
				countit++;
				if(maxit > 0) if(countit >= maxit) running = false;				
			}
			while(running);
			
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 4\n";
			out = newdat;
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 5\n";
		}

	private:
		T (*FA)(T);

	private:
		oct::math::Vector<T*> outputs;
		oct::math::Vector<T> gradient;
		oct::math::Vector<T> gradient_unit;
		//oct::math::Vector<datatype> gradient_descent;
	};


}


#endif