
#ifndef OCTETOS_NEURONAL_HH
#define OCTETOS_NEURONAL_HH

#include <vector>
#include <octetos/core/Exception.hh>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

namespace oct::neu
{
	typedef unsigned int Index;

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
	

}
namespace oct::core
{
	
}

#endif