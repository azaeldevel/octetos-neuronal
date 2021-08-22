
#ifndef OCTETOS_PLOT_PLOT_HH
#define OCTETOS_PLOT_PLOT_HH


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace oct::math
{

class Plot
{
public:
	Plot();
	~Plot();

	
	void plotting(const std::string& datafile);
	void plotting(const std::vector<std::vector<double>>&);
	void plotting(const std::vector<double>&);
	void plotting(const std::vector<std::vector<float>>&);

	void set_title(const std::string& title);
	void set_styleline(unsigned int);
private:
	FILE* gnuplotPipe;
	unsigned int sl;
};

}

#endif