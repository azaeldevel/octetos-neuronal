

#ifndef OCTETOS_NEURONAL_LINE_HH
#define OCTETOS_NEURONAL_LINE_HH


#include <fstream>


#include "neuronal.hh"

namespace oct::neu
{
	/**
	*\brief Genera una distribucion de que podria interpretarse como una recta
	*/
	template<typename T> class Line
	{
	public:
		/**
		*\param maxerr
		*\param minerr
		*/
		Line(T m, T b, T maxerr, T minerr, Index count, T xmin, T xmax)
		{
			ds.resize(count);
			for(std::vector<T>& d : ds)
			{
				d.resize(2);
			}

			//
			T distr = T(1) / (maxerr - minerr);
			T randPos;
			for(std::vector<T>& d : ds)
			{
				d[0] = randNumber(xmin,xmax);
				d[1] = (m * d[0]) + b;
				randPos = randNumber();
				if(randPos > 0.5) d[1] = d[1] + (randNumber() * distr);
				else d[1] = d[1] - (randNumber() * distr);
			}
		}

		void print() const
		{
			for(const std::vector<T>& d : ds)
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
			for(const std::vector<T>& d : ds)
			{
				out << d[0] << " " << d[1] << "\n";
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
	private:
		std::vector<std::vector<T>> ds;
	};
}


#endif