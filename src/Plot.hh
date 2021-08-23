
#ifndef OCTETOS_PLOT_PLOT_HH
#define OCTETOS_PLOT_PLOT_HH


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>

namespace oct::math
{

class Plot
{
public:
	Plot();
	~Plot();

	
	void plotting(const std::string& datafile);
	void plotting(const std::vector<std::vector<double>>&);
	void plotting(const std::list<std::vector<double>>&);
		
	void set_title(const std::string& title);
	void set_styleline(unsigned int);
	void set_noautotitles();
	void set_terminal(const std::string& term);
	void set_output(const std::string& out);
	void set_label(const std::string& , unsigned int, unsigned int );
	void set_label(const std::string& , double, double );
private:
	FILE* gnuplotPipe;
	unsigned int sl;
};

}

#endif