

#include "neurona.hh"
#include "plot.hh"

int main()
{
	oct::math::Plotter plotter;
	std::ofstream file("circle.dat");
	
	std::vector<double> data(2);
	oct::neu::Neurona<double> per(2);
	data[0] = 0.0;
	data[1] = 1.0;
	per.get_inputs()[0] = &data[0];
	per.get_inputs()[1] = &data[1];
	per.get_weight()[0] = data[0];
	per.get_weight()[1] = data[1];
	double delta = 0.01;
	for(unsigned int i = 0; i < 100; i++ )
	{
		data[0] += delta;
		per.get_weight()[0] += delta;
		data[1] -= delta;
		per.get_weight()[1] -= delta;
		per.spread(oct::neu::ActivationFuntion::IDENTITY);
		//std::cout << "Salida 1 = " << per.get_out() << std::endl;
		plotter.save(file,data[0], data[1], per.get_out());
	}
	for(unsigned int i = 0; i < 100; i++ )
	{
		data[0] -= delta;
		per.get_weight()[0] -= delta;
		data[1] += delta;
		per.get_weight()[1] += delta;
		per.spread(oct::neu::ActivationFuntion::IDENTITY);
		//std::cout << "Salida 2 = " << per.get_out() << std::endl;
		plotter.save(file,data[0], data[1], per.get_out());
	}	
	for(unsigned int i = 0; i < 100; i++ )
	{
		data[0] -= delta;
		per.get_weight()[0] -= delta;
		data[1] -= delta;
		per.get_weight()[1] -= delta;
		per.spread(oct::neu::ActivationFuntion::IDENTITY);
		//std::cout << "Salida 1 = " << per.get_out() << std::endl;
		plotter.save(file,data[0], data[1], per.get_out());
	}
	for(unsigned int i = 0; i < 100; i++ )
	{
		data[0] -= delta;
		per.get_weight()[0] -= delta;
		data[1] += delta;
		per.get_weight()[1] += delta;
		per.spread(oct::neu::ActivationFuntion::IDENTITY);
		//std::cout << "Salida 1 = " << per.get_out() << std::endl;
		plotter.save(file,data[0], data[1], per.get_out());
	}
		
	
}