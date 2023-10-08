
#include <iostream>

#include "v0.hh"

#include <neuronal/1/Neurona.hh>

namespace neuronal = oct::neu::v0;


int v0_init(void)
{

	return 0;
}
int v0_clean(void)
{

	return 0;
}
void v0_developing()
{
    neuronal::Neurona<float> neu1;

    neuronal::Cumulus<float> cum1(4,2,10,5);
    neuronal::Cumulus<float> cum2(2,4,10,5);
    /*for(size_t i = 0; i < 10; i++)
    {
        std::cout << "\nCapa " << i << "\n";
        std::cout << "\tsize : " << cum1[i].size() << "\n";
    }*/

}
