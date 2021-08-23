
#include <iostream>


#include "Plot.hh"


namespace oct::math
{
	Plot::Plot()
	{
		gnuplotPipe = popen("gnuplot -persistent","w");
		sl = 7;
	}
	Plot::~Plot()
	{
		pclose(gnuplotPipe);
	}
	void Plot::plotting(const std::string& datafile)
	{
		std::string cmd = "plot '" ;
		cmd += datafile + "'\n";
		fprintf(gnuplotPipe,"%s \n",cmd.c_str());
	}	
	void Plot::plotting(const std::vector<std::vector<double>>& ps)
	{
		fprintf(gnuplotPipe,"plot ");
		for(unsigned int i = 0; i < ps.size(); i++)
		{
			fprintf(gnuplotPipe,"'-' w p ls %i",sl);
			if(i < ps.size()-1) fprintf(gnuplotPipe,",");
		}
		fprintf(gnuplotPipe,"\n");
		for(unsigned int i = 0; i < ps.size(); i++)
		{
			fprintf(gnuplotPipe,"%f\t%f\n",ps[i][0],ps[i][1]);
			//std::cout << ps[i][0] << "," << ps[i][2] << "\n";
			fprintf(gnuplotPipe,"e\n");
		}			
	}	
	void Plot::plotting(const std::list<std::vector<double>>& ps)
	{
		fprintf(gnuplotPipe,"plot ");
		for(unsigned int i = 0; i < ps.size(); i++)
		{
			fprintf(gnuplotPipe,"'-' w p ls %i",sl);
			if(i < ps.size()-1) fprintf(gnuplotPipe,",");
		}
		fprintf(gnuplotPipe,"\n");
		for(const std::vector<double>& point : ps)
		{
			fprintf(gnuplotPipe,"%f\t%f\n",point[0],point[1]);
			//std::cout << ps[i][0] << "," << ps[i][2] << "\n";
			fprintf(gnuplotPipe,"e\n");
		}			
	}
	
	void Plot::set_title(const std::string& title)
	{
		std::string cmd = "set title ";
		cmd += "\"" + title + "\"\n";		
		fprintf(gnuplotPipe,"%s\n",cmd.c_str());
	}
	void Plot::set_styleline(unsigned int sl)
	{
		this->sl = sl;
	}
	void Plot::set_noautotitles()
	{
		fprintf(gnuplotPipe,"set key noautotitle\n");
	}
	void Plot::set_terminal(const std::string& term)
	{
		fprintf(gnuplotPipe,"set terminal %s\n",term.c_str());
	}
	void Plot::set_output(const std::string& out)
	{
		fprintf(gnuplotPipe,"set output '%s'\n",out.c_str());
	}
}