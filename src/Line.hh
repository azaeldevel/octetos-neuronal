

#ifndef OCTETOS_NEURONAL_LINE_HH
#define OCTETOS_NEURONAL_LINE_HH


#include <fstream>


#include "neuronal.hh"

namespace oct::neu
{
	/**
	*\brief Genera una distribucion de que podria interpretarse como una recta
	*/
	template<typename T> class Line : public std::vector<Data<T>>
	{
	public:
		/**
		*\param maxerr
		*\param minerr
		*/
		Line(T m, T b, T derr, Index count, T xmin, T xmax)
		{
			std::vector<Data<T>>::resize(count);
			for(Data<T>& d : *this)
			{
				d.inputs.resize(2);
				d.outputs.resize(1);
			}

			//
			T randPos,randSensor,errOut;
			for(Index i = 0; i < std::vector<Data<T>>::size(); i++)
			{
				//valores aceptables
				std::vector<Data<T>>::at(i).inputs[0] = randNumber(xmin,xmax);
				std::vector<Data<T>>::at(i).inputs[1] = (m * std::vector<Data<T>>::at(i).inputs[0]) + b;
				std::vector<Data<T>>::at(i).outputs[0] = 1.0;//aceptable
				randPos = randNumber();
				randSensor = randNumber();
				if(randPos > 0.5) std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i).inputs[1] + (derr * randSensor);
				else std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i).inputs[1] - (derr * randSensor);
				//valores no aceptables
				i++;
				errOut = randNumber(1.0,20.0);
				std::vector<Data<T>>::at(i).inputs[0] = std::vector<Data<T>>::at(i-1).inputs[0];
				std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i-1).inputs[1];//la misma ordena pero cun error fuera de rango
				std::vector<Data<T>>::at(i).outputs[0] = 0.0;//no captable		
				if(randPos > 0.5) std::vector<Data<T>>::at(i-1).inputs[1] = std::vector<Data<T>>::at(i-1).inputs[1] + (derr * errOut);
				else std::vector<Data<T>>::at(i-1).inputs[1] = std::vector<Data<T>>::at(i-1).inputs[1] - (derr * errOut);		
			}
		}

		void print() const
		{
			for(const std::vector<T>& d : *this)
			{			
				std::cout << "(";
				std::cout << d[0];
				std::cout << ",";
				std::cout << d[1];
				std::cout << ")\n";
			}
		}
		void dating() const
		{
			std::ofstream out("Line.data");
			for(const Data<T>& d : *this)
			{
				out << d.inputs[0] << " " << d.inputs[1] << "\n";
			}
			out.flush();
			out.close();
		}
		void plot() const
		{
			FILE *p = popen("gnuplot", "w");

	  		fprintf(p, "set title \"Cavendish Data\"\n");
			fprintf(p, "set xlabel \"Time (s)\"\n");
			fprintf(p, "set ylabel \"Angle (mrad)\"\n");
	  		fprintf(p, "set grid\n");
			fprintf(p, "plot ");
			fprintf(p, "\"Line.data\"");
			fprintf(p, " title \"\"\n");

	  		pclose(p);
		}
	};
}


#endif