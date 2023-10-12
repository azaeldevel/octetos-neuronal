
#include <iostream>

#include "v0.hh"

#include <cmath>

#include <neuronal/1/Perceptron.hh>

namespace neuronal = oct::neu::v0;
namespace core = oct::core::v3;

int v0_init(void)
{

	return 0;
}
int v0_clean(void)
{

	return 0;
}



float fun1(float d)
{
    return d;
}
float dev1(float d)
{
    return 1;
}

void v0_developing_old()
{

}

void v0_developing()
{
    neuronal::Perceptron<float> pers1(5,fun1,dev1);
}



