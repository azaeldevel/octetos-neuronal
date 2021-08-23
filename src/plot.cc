


#include "Plot.hh"

int main() 
{
	oct::math::Plot plot1;
	plot1.set_title("Testing plot..");
	std::vector<std::vector<double>> ps(10);
	for(unsigned int i = 0; i < ps.size(); i++)
	{
		ps[i].resize(2);
		ps[i][0] = i;
		ps[i][1] = i;
	}
	plot1.set_styleline(1);
	plot1.plotting(ps);
	std::vector<double> point4(2);
	point4[0] = 4;
	point4[1] = 5;
	/*plot1.plotting(point4);
	std::vector<double> point5(2);
	point5[0] = 4;
	point5[1] = 5;
	plot1.plotting(point5);*/
	return 0;
}