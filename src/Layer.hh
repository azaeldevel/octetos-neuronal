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
		Layer(unsigned short inputsP, unsigned short countP, T (*fa)(T),T (*d)(T)) : std::vector<Perceptron<T>>(countP),FA(fa),D(d)
		{
			set(inputsP,countP,fa,d);
		}
		void set(unsigned short inputsP, unsigned short countP, T (*fa)(T),T (*d)(T))
		{
			if(std::vector<Perceptron<T>>::size() < countP) std::vector<Perceptron<T>>::resize(countP);
			
			FA = fa;
			D = d;
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
				std::vector<Perceptron<T>>::at(i).spread(FA);			
			}
			//std::cout << "\tvoid Layer::spread(): step 2 \n";
		}
		void gd(unsigned short maxit, T ratio, Layer& prevL, std::vector<T>& expected)
		{
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 1\n";
			//if(outputs.size() != gradient_unit.size()) throw octetos::core::Exception("Los vectores son de diferente tamano",__FILE__,__LINE__);
			
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 2\n";
			bool running = true;
			unsigned short countit = 0;
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 3\n";
			
			do
			{
				gd(prevL,ratio,expected);
				countit++;
				if(maxit > 0) if(countit >= maxit) running = false;				
			}
			while(running);
			
			//std::cout << "void minimize(unsigned short maxit, datatype ratio,oct::math::Vector<datatype>& out) Step 4\n";
			
		}

		void gd(Layer& prevL, T ratio, std::vector<T>& expected)
		{
			for(Index i = 0; i < prevL.size(); i++)
			{
				(*prevL.get_outputs()[i]) = (*prevL.get_outputs()[i]) - (ratio * D(*prevL.get_outputs()[i]));
			}	
			
			spread();
			
			for(Index i = 0; i < std::vector<Perceptron<T>>::size(); i++)
			{
				std::vector<Perceptron<T>>::at(i).gd(expected[i]);
			}
			
			print(outputs);
			std::cout << "\n";		
		}
		

	private:
		T (*FA)(T);
		T (*D)(T);

	private:
		std::vector<T*> outputs;
	};


}


#endif