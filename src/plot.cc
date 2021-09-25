
#include <iostream>
#include <fstream>
#include <octetos/core/Exception.hh>

#include "plot.hh"


namespace oct::math
{
	Plotter::Plotter()
	{
		gnuplotPipe = popen("gnuplot -persistent","w");
		sl = 7;
	}
	Plotter::~Plotter()
	{
		pclose(gnuplotPipe);
	}
	void Plotter::plottingFile2D(const std::string& datafile)
	{
		std::string cmd = "plot '" ;
		cmd += datafile + "'\n";
		fprintf(gnuplotPipe,"%s\n",cmd.c_str());
	}	
	void Plotter::plotting(const std::vector<std::vector<double>>& ps)
	{		
		if(ps.size() == 0) return;
		
		int dimension = (*ps.begin()).size();
		
		fprintf(gnuplotPipe,dimension == 2 ? "plot " : "splot ");
		for(unsigned int i = 0; i < ps.size(); i++)
		{
			fprintf(gnuplotPipe,"'-' w p ls %i",sl);
			if(i < ps.size()-1) fprintf(gnuplotPipe,",");
		}
		fprintf(gnuplotPipe,"\n");
		for(const std::vector<double>& point : ps)
		{
			if(point.size() != dimension) throw oct::core::Exception("La dimension no coincide",__FILE__,__LINE__);
			if(point.size() == 2)fprintf(gnuplotPipe,"%f\t%f\n",point[0],point[1]);
			else if(point.size() == 3)fprintf(gnuplotPipe,"%f\t%f%f\n",point[0],point[1],point[2]);
			else throw oct::core::Exception("Dimension no manejable",__FILE__,__LINE__);
			//std::cout << ps[i][0] << "," << ps[i][2] << "\n";
			fprintf(gnuplotPipe,"e\n");
		}		
	}	
	void Plotter::plotting(const std::list<std::vector<double>>& ps)
	{
		if(ps.size() == 0) return;
		
		int dimension = (*ps.begin()).size();
		
		fprintf(gnuplotPipe,dimension == 2 ? "plot " : "splot ");
		for(unsigned int i = 0; i < ps.size(); i++)
		{
			fprintf(gnuplotPipe,"'-' w p ls %i",sl);
			if(i < ps.size()-1) fprintf(gnuplotPipe,",");
		}
		fprintf(gnuplotPipe,"\n");
		for(const std::vector<double>& point : ps)
		{
			if(point.size() != dimension) throw oct::core::Exception("La dimension no coincide",__FILE__,__LINE__);
			if(point.size() == 2)fprintf(gnuplotPipe,"%f\t%f\n",point[0],point[1]);
			else if(point.size() == 3)fprintf(gnuplotPipe,"%f\t%f%f\n",point[0],point[1],point[2]);
			else throw oct::core::Exception("Dimension no manejable",__FILE__,__LINE__);
			//std::cout << ps[i][0] << "," << ps[i][2] << "\n";
			fprintf(gnuplotPipe,"e\n");
		}			
	}
	
	void Plotter::set_title(const std::string& title)
	{
		std::string cmd = "set title ";
		cmd += "\"" + title + "\"\n";		
		fprintf(gnuplotPipe,"%s\n",cmd.c_str());
	}
	void Plotter::set_window_title(const std::string& title)
	{
		std::string cmd = "set terminal windows title ";
		cmd += "\"" + title + "\"\n";		
		fprintf(gnuplotPipe,"%s\n",cmd.c_str());
	}
	void Plotter::set_styleline(unsigned int sl)
	{
		this->sl = sl;
	}
	void Plotter::set_noautotitles()
	{
		fprintf(gnuplotPipe,"set key noautotitle\n");
	}
	void Plotter::set_terminal(const std::string& term)
	{
		fprintf(gnuplotPipe,"set terminal %s\n",term.c_str());
	}
	void Plotter::set_output(const std::string& out)
	{
		fprintf(gnuplotPipe,"set output '%s'\n",out.c_str());
	}
	void Plotter::set_label(const std::string& text, unsigned int x, unsigned int y)
	{
		fprintf(gnuplotPipe,"set label \"%s\" at %i,%i\n",text.c_str(),x,y);
	}
	void Plotter::set_label(const std::string& text, double x, double y)
	{
		fprintf(gnuplotPipe,"set label \"%s\" at %f,%f\n",text.c_str(),x,y);
	}
	
	
	
	
	
	void Plotter::save(std::ofstream& file, double x, double y)
	{
		file << x << "\t" << y << "\n";
	}
	void Plotter::save(std::ofstream& file, double x, double y, double z)
	{
		file << x << "\t" << y << "\t" << z<< "\n";
	}
}