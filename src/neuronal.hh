
#ifndef OCTETOS_NEURONAL_HH
#define OCTETOS_NEURONAL_HH

#include <vector>
#include <octetos/core/Exception.hh>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "plot.hh"

#define DATATYPE double


namespace oct::neu
{
	typedef unsigned int Index;
	typedef DATATYPE real;

	template<typename T> struct Data
	{
		std::vector<T> inputs;
		std::vector<T> outputs;
	};

	template<typename T> void print(const std::vector<T>& v,std::string& out)
	{
		out += "(";
		for(unsigned short i = 0; i < v.size(); i++)
		{
			out += std::to_string(v[i]);
			if(i < v.size() - 1) out += ",";
		}
		out += ")";
	}
		template<typename T> void print(const std::vector<T*>& v)
		{
			std::cout << "(";
			for(unsigned short i = 0; i < v.size(); i++)
			{
				std::cout << *v[i];
				if(i < v.size() - 1) std::cout << ",";
			}
			std::cout << ")";
		}
		template<typename T> void print(const std::vector<T*>& v, std::string& out)
		{
			out += "(";
			for(unsigned short i = 0; i < v.size(); i++)
			{
				out += std::to_string(*v[i]);
				if(i < v.size() - 1) out += ",";
			}
			out += ")";
		}
	template<typename T> void print(const std::vector<T>& v)
	{
		std::cout << "(";
		for(unsigned short i = 0; i < v.size(); i++)
		{
			std::cout << std::setprecision(10) << v[i];
			if(i < v.size() - 1) std::cout << ",";
		}
		std::cout << ")";
	}
	
	template<typename T> struct Learning
	{
		T ratio;
		T p;
		T r;
		bool variable;
		T epsilon; 
		unsigned int iterations;
		unsigned int training;

		Learning()
		{
			p = 1.1;
			r = 0.5;
			variable = false;
		}
	};
	template<typename T> struct Plotting
	{
		oct::math::Plotter plotter;
		std::list<std::vector<T>> data;
		T last;
		std::string filename;
			
		Plotting()
		{
			last = T(0);
		}
	};
}
namespace oct::core
{
	
}

#endif