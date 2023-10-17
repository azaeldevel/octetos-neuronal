
#include <iostream>

#include "v0.hh"

#include <cmath>

#include <neuronal/1/Perceptron.hh>
#include <neuronal/1/Backp.hh>

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

float init_weights_pers1(size_t n,size_t w)
{
    return 0;
}
float init_bias_pers1(size_t n)
{
    return 0;
}

void v0_developing_old()
{

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
void v0_developing()
{

    core::array<core::array<float>> bach1I;
    core::array<core::array<float>> bach1O;
    fill_bach_1(bach1I,bach1O,10);
    neuronal::numbers::matrix<float> mx1;
    /*for(size_t i = 0; i < bach1I.size(); i++)
    {
        std::cout << "\n";
        mx1.buffer(bach1I[i].size(),1,(float*)(bach1I[i]));
        mx1.print(std::cout);
    }*/


    core::array<float> in1(3);
    in1[0] = 0.5f;
    in1[1] = 0.5f;
    in1[2] = 0.5f;
    neuronal::Perceptron<float> pers1(3,fun1,dev1,0.5f,0.0f);
    pers1.spread(in1);
    //pers1.output().outputs.print(std::cout);
    CU_ASSERT(core::equal(pers1.output().outputs[0][0],0.75f))

    neuronal::Perceptron<float> pers2(3,2,5,4,fun1,dev1,0.0f,0.0f);
    pers2.spread(bach1I);

    neuronal::Backp<float> back1(bach1I,bach1O,pers2);
    float e1 = back1.cost();
    std::cout << "Error : " << e1 << "\n";

}



