

#ifndef OCTETOS_PLOT_PLOT_HH
#define OCTETOS_PLOT_PLOT_HH


#include <stdlib.h>
#include <stdio.h>
#include <string>

namespace oct
{

class Plot
{
public:
	Plot();
	void plotting(const std::string& datafile);

	void set_title(const std::string& title);
private:
	FILE* gnuplotPipe;
};

}

#endif