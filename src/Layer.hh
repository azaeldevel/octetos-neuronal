#ifndef OCTETOS_NEURONAL_LAYER_HH
#define OCTETOS_NEURONAL_LAYER_HH

#include <Perceptron.hh>
#include <list>

namespace oct::neu
{
	struct Model
	{
		unsigned short height;
		ActivationFuntion AF;
		unsigned short inputsNeurona;
	};

	template<typename T> class Layer : public std::vector<Perceptron<T>>
	{
	public:
		Layer()
		{		
		}
		void set(unsigned short inputsP, unsigned short countP, ActivationFuntion af)
		{
			if(std::vector<Perceptron<T>>::size() < countP) std::vector<Perceptron<T>>::resize(countP);
			
			//AF = af;
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
			//dEdW.resize(outputs.size());
			//dEdZ.resize(outputs.size());
			//gradientDescent.resize(outputs.size());
			//dRdW.resize(outputs.size());
			//En.resize(outputs.size());
			//En1.resize(outputs.size());
		}
		void set(Model& model)
		{
			this->model = &model;
			if(std::vector<Perceptron<T>>::size() < model.height) std::vector<Perceptron<T>>::resize(model.height);
			for(Perceptron<T>& p : *this)
			{
				p.set(model.inputsNeurona);
			}
			if(outputs.size() < model.height) outputs.resize(model.height);
			for(unsigned short i = 0; i < model.height; i++)
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
				std::vector<Perceptron<T>>::at(i).spread(model->AF);			
			}
			//std::cout << "\tvoid Layer::spread(): step 2 \n";
		}
		/**
		*\brief 
		*/
		void bp(unsigned short maxit, T ratio, const std::vector<T>& dEdW)
		{
			//Index highIndex = max(dEdZ);//neurona de mayor responsabilidad
			//wlakingDown(highIndex,ratio);
		}
		

	private:

	private:
		std::vector<T*> outputs;
		//ActivationFuntion AF;
		//std::vector<T> dEdR,dRdZ,dZdW,dEdW,dEdZ,dRdW;
		//std::vector<T> En,En1,,gradientDescent
		const Model* model;
	};

}


#endif