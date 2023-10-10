
#include <iostream>

#include "v0.hh"

#include <neuronal/1/Neurona.hh>

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

void init_default(neuronal::Cumulus<float>& cumulo)
{
    for(size_t i = 0; i < cumulo.size() ; i++)//layer
    {
        for(size_t j = 0; j < cumulo.layer(i).size() ; j++)//neuronas
        {
            for(size_t k = 0; k < cumulo.layer(i).at(j).size() ; k++)//inputs
            {
                cumulo.layer(i).at(j).at(k).weight = 0.5f;
            }
        }
    }
}
void v0_developing()
{
    neuronal::Neurona<float> neu1;

    //float init_valur = 0.5;
    neuronal::Cumulus<float> cum1(4,2,10,5);
    init_default(cum1);
    neuronal::Cumulus<float> cum2(2,4,10,5);
    init_default(cum2);
    /*for(size_t i = 0; i < 10; i++)
    {
        std::cout << "\nCapa " << i << "\n";
        std::cout << "\tsize : " << cum1[i].size() << "\n";
    }*/
    core::array<float> ins1 = {2.0f,2.0f};
    neuronal::Cumulus<float> cum3(2,2,6,2);
    init_default(cum3);
    cum3.spread(ins1);
    std::cout << "\tinput : " << *cum1.output().at(0).at(0).input << "\n";
    std::cout << "\toutput : " << cum1.output().at(0).output << "\n";
    for(size_t i = 0; i < cum1.layers(); i++)
    {
        for(size_t j = 0; j < cum1.layer(i).size(); j++)
        {
            std::cout << "neurona : " << i << "," << j << "\n";
            std::cout << "\toutput : " << cum1.layer(i).at(j).output << "\n";
        }
    }

}
