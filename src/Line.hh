

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
		Line(T x0, T y0, T x1, T y1,T derr, Index count, unsigned int type)
		{
			T m = (y1 - y0)/(x1 -x0);
			T b = y0;
			T xmin = x0;
			T xmax = x1;
			std::vector<Data<T>>::resize(count);
			for(Data<T>& d : *this)
			{
				d.inputs.resize(2);
				d.outputs.resize(1);
			}
			
			if( type == 1)
			{
				for(Index i = 0; i < std::vector<Data<T>>::size(); i++)
				{
					line(x0,y0,x1,y1,derr,m,i);
				}
			}
			else if( type == -1)
			{
				for(Index i = 0; i < std::vector<Data<T>>::size(); i++)
				{
					garbage(x0,y0,x1,y1,i);
				}
			}
			else if( type == 0)
			{
				for(Index i = 0; i < std::vector<Data<T>>::size(); i++) //uno si y uno no
				{
					T randCerteza = core::randNumber();
					bool certeza = randCerteza > 0.5 ? true : false;
					if(certeza)
					{
						//valores aceptables
						line(x0,y0,x1,y1,derr,m,i);
					}
					else
					{
						garbage(x0,y0,x1,y1,i);
					}
				}
			}
			else
			{
				throw core::Exception("Bandera desconocida",__FILE__,__LINE__);
			}
		}
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
			T randPos,randSensor,errOut,randCerteza;
			bool certeza;
			for(Index i = 0; i < std::vector<Data<T>>::size(); i++) //uno si y uno no
			{
				randCerteza = core::randNumber();
				certeza = randCerteza > 0.5 ? true : false;
				//std::cout << "bool " << certeza << "\n";
				if(certeza)
				{
					//valores aceptables
					std::vector<Data<T>>::at(i).inputs[0] = core::randNumber(xmin,xmax);
					std::vector<Data<T>>::at(i).inputs[1] = (m * std::vector<Data<T>>::at(i).inputs[0]) + b;
					std::vector<Data<T>>::at(i).outputs[0] = 1.0;//aceptable
					randPos = core::randNumber();
					randSensor = core::randNumber();
					if(randPos > 0.5) std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i).inputs[1] + (derr * randSensor);
					else std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i).inputs[1] - (derr * randSensor);
				}
				else
				{
					//errOut = randNumber(1.0,100.0);
					std::vector<Data<T>>::at(i).inputs[0] = core::randNumber(xmin - (xmin * 10.0),xmax * 10.0);
					std::vector<Data<T>>::at(i).inputs[1] = core::randNumber(xmin - (xmin * 10.0),xmax * 10.0);//la misma ordena pero cun error fuera de rango
					T tmepPoint = (m * std::vector<Data<T>>::at(i).inputs[0]) + b;
					if(std::abs(tmepPoint - std::vector<Data<T>>::at(i).inputs[1]) < 0.01)
					{
						std::vector<Data<T>>::at(i).inputs[1] = core::randNumber(xmin - (xmin *10.0),xmax * 10.0);
					}
					std::vector<Data<T>>::at(i).outputs[0] = 0.0;//no captable		
					//if(randPos > 0.5) std::vector<Data<T>>::at(i-1).inputs[1] = std::vector<Data<T>>::at(i-1).inputs[1] + (derr * errOut);
					//else std::vector<Data<T>>::at(i-1).inputs[1] = std::vector<Data<T>>::at(i-1).inputs[1] - (derr * errOut);
				}
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

		void line(T x0, T y0, T x1, T y1, T derr,T m,Index i)
		{
			std::vector<Data<T>>::at(i).inputs[0] = core::randNumber(x0,x1);
			std::vector<Data<T>>::at(i).inputs[1] = (m * std::vector<Data<T>>::at(i).inputs[0]) + y0;
			std::vector<Data<T>>::at(i).outputs[0] = 1.0;//aceptable
			T sensorErr = core::randNumber(0.0,derr);
			if(sensorErr > 0.5) 
			{
				std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i).inputs[1] + core::randNumber(0,derr);
			}
			else 
			{
				std::vector<Data<T>>::at(i).inputs[1] = std::vector<Data<T>>::at(i).inputs[1] - core::randNumber(0,derr);
			}
		}
		void garbage(T x0, T y0, T x1, T y1,Index i)
		{
			std::vector<Data<T>>::at(i).inputs[0] = core::randNumber(x0,x1);
			std::vector<Data<T>>::at(i).inputs[1] = core::randNumber(y0,y1);
			std::vector<Data<T>>::at(i).outputs[0] = 0.0;//no captabl
		}
	};
}


#endif