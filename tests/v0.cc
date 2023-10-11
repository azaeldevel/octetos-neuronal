
#include <iostream>

#include "v0.hh"

#include <cmath>

#include <neuronal/1/Neurona.hh>
#include <neuronal/1/backp.hh>

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

void fill_bach_1(core::array<core::array<float>>& in,core::array<core::array<float>>& out,size_t size)
{
    size_t sub = 6;
    size = size/sub;
    size = size*sub;
    in.resize(size);
    out.resize(size);

    float actual = 0,delta = 0.001,err = 0.01;
    for(size_t i = 0; i < size + sub;i += sub)
    {
        if(i == size) break;
        //std::cout << "\tIndice  : " << i << " de " << size <<  "\n";
        //correct
        in[i + 0].resize(3);
        out[i + 0].resize(2);
        in[i + 0][0] = actual;
        in[i + 0][1] = std::sin(actual);
        in[i + 0][2] = std::cos(actual);
        out[i + 0][0] = 1.0f;
        out[i + 0][1] = 0.0f;

        //maybe
        in[i + 1].resize(3);
        out[i + 1].resize(2);
        in[i + 1][0] = actual;
        in[i + 1][1] = std::sin(actual) + err;
        in[i + 1][2] = std::cos(actual) + err;
        out[i + 1][0] = 0.5f;
        out[i + 1][1] = 0.5f;

        //fails
        in[i + 2].resize(3);
        out[i + 2].resize(2);
        in[i + 2][0] = actual;
        in[i + 2][1] = std::sin(actual) + 2 * err;
        in[i + 2][2] = std::cos(actual) + 2 * err;
        out[i + 2][0] = 0.0f;
        out[i + 2][1] = 1.0f;

        in[i + 3].resize(3);
        out[i + 3].resize(2);
        in[i + 3][0] = actual;
        in[i + 3][1] = std::sin(actual) + 3 * err;
        in[i + 3][2] = std::cos(actual) + 3 * err;
        out[i + 3][0] = 0.0f;
        out[i + 3][1] = 1.0f;

        in[i + 4].resize(3);
        out[i + 4].resize(2);
        in[i + 4][0] = actual;
        in[i + 4][1] = std::sin(actual) + 5 * err;
        in[i + 4][2] = std::cos(actual) + 5 * err;
        out[i + 4][0] = 0.0f;
        out[i + 4][1] = 1.0f;

        in[i + 5].resize(3);
        out[i + 5].resize(2);
        in[i + 5][0] = actual;
        in[i + 5][1] = std::sin(actual) + 10 * err;
        in[i + 5][2] = std::cos(actual) + 10 * err;
        out[i + 5][0] = 0.0f;
        out[i + 5][1] = 1.0f;

        actual += delta;
    }

}

float fun1(float d)
{
    return d;
}
float dev1(float d)
{
    return 1;
}

void v0_developing()
{
    //neuronal::Neurona<float> neu1;

    core::array<float> ins1 = {0.5f,0.5f};

    neuronal::Cumulus<float> cum1(2,2,6,2,fun1,dev1);
    init_default(cum1);
    cum1.spread(ins1);
    //std::cout << "\n\tinput : " << *cum1.output().at(0).at(0).input << "\n";
    //std::cout << "\toutput : " << cum1.output().at(0).output << "\n";
    /*
    for(size_t i = 0; i < cum1.layers(); i++)
    {
        for(size_t j = 0; j < cum1.layer(i).size(); j++)
        {
            std::cout << "neurona : " << i << "," << j << "\n";
            std::cout << "\toutput : " << cum1.layer(i).at(j).output << "\n";
        }
    }
    */
    CU_ASSERT(core::equal(cum1.output().at(0).output,0.5f))
    CU_ASSERT(core::equal(cum1.output().at(1).output,0.5f))

    core::array<float> ins2 = {0.5f,0.5f,0.5f};
    neuronal::Cumulus<float> cum2(3,3,3,3,fun1,dev1);
    init_default(cum2);
    cum2.spread(ins2);
    /*
    for(size_t i = 0; i < 10; i++)
    {
        std::cout << "\nCapa " << i << "\n";
        std::cout << "\tsize : " << cum1[i].size() << "\n";
    }
    */
    //std::cout << "\n\tinput : " << *cum2.output().at(0).at(0).input << "\n";
    //std::cout << "\toutput : " << cum2.output().at(0).output << "\n";
    /*
    std::cout << "\n";
    for(size_t i = 0; i < cum2.layers(); i++)
    {
        for(size_t j = 0; j < cum2.layer(i).size(); j++)
        {
            std::cout << "neurona : " << i << "," << j << "\n";
            std::cout << "\toutput : " << cum2.layer(i).at(j).output << "\n";
        }
    }
    */
    CU_ASSERT(core::equal(cum2.output().at(0).output,1.6875f))
    CU_ASSERT(core::equal(cum2.output().at(1).output,1.6875f))

    std::cout << "\n";
    neuronal::Cumulus<float> cum3(3,2,5,3,fun1,dev1);
    init_default(cum3);
    core::array<core::array<float>> bach1I;
    core::array<core::array<float>> bach1O;
    fill_bach_1(bach1I,bach1O,1000);
    neuronal::Backp<float> back1(cum3,bach1I,bach1O,1.0e-5f);
    float E;
    for(size_t i = 0; i < 100; i++)
    {
        E = back1.training();
        std::cout << "E = " << E << "\n";
        std::cout << "\n\n";

    }
}
