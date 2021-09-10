#ifndef OCTETOS_NEURONAL_LAYER_HH
#define OCTETOS_NEURONAL_LAYER_HH

#include "neurona.hh"
#include <list>

namespace oct::neu
{
	struct Model
	{
		unsigned short height;
		ActivationFuntion AF;
		unsigned short inputsNeurona;
	};

	template<typename T> class Layer : public std::vector<Neurona<T>>
	{
	public:
		Layer()
		{
		}
		void set(Model& model)
		{
			this->model = &model;
			if(std::vector<Neurona<T>>::size() < model.height) std::vector<Neurona<T>>::resize(model.height);
			for(Neurona<T>& p : *this)
			{
				p.set(model.inputsNeurona);
			}
			if(outputs.size() < model.height) outputs.resize(model.height);
			for(unsigned short i = 0; i < model.height; i++)
			{//asigna la salida de cada perceptor al vetor de salida
				outputs[i] = &std::vector<Neurona<T>>::at(i).get_out(); 
			}
		}
		std::vector<T*>& get_outputs()
		{
			return outputs;
		}

		void spread()
		{
			//std::cout << "\tvoid Layer::spread(): step 1 \n";
			for(unsigned short i = 0; i < std::vector<Neurona<T>>::size(); i++)
			{
				//std::cout << "\tvoid Layer::spread(): step 1.1\n";
				std::vector<Neurona<T>>::at(i).spread(model->AF);			
			}
			//std::cout << "\tvoid Layer::spread(): step 2 \n";
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