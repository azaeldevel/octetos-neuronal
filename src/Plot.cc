
#include "Plot.hh"


namespace oct
{
	Plot::Plot()
	{
		gnuplotPipe = popen("gnuplot -persistent","w");
	}
	void Plot::plotting(const std::string& datafile)
	{
		std::string cmd = "plot '" ;
		cmd += datafile + "'\n";
		fprintf(gnuplotPipe,"%s \n",cmd.c_str());
	}
	
	
	void Plot::set_title(const std::string& title)
	{
		std::string cmd = "set title ";
		cmd += "\"" + title + "\"\n";		
		fprintf(gnuplotPipe,"%s \n",cmd.c_str());
	}
}