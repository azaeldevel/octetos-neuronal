
#include <iostream>

#include "v1.hh"

#include <cmath>

#include <neuronal/1/Perceptron.hh>
#include <neuronal/1/Backp.hh>
#include <limits>
#include <cmath>

namespace neuronal = oct::neu::v0;
namespace core = oct::core::v3;

int v1_init(void)
{

	return 0;
}
int v1_clean(void)
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







template<core::number T> class BachGates
{
public:
    enum Gate
    {
        NONE,
        AND,
        OR
    };

public:
    BachGates(T e,Gate g) : error(e),gate(g),dist_error(0,e),dist_data(0,3)
    {
    }

    void generate(core::array<core::array<float>>& in_bach,core::array<core::array<float>>& out_bach,size_t amoung)
    {
        switch(gate)
        {
        case Gate::AND:
            and_gate(in_bach,out_bach,amoung);
            break;
        }
    }
    void and_gate(core::array<core::array<float>>& in_bach,core::array<core::array<float>>& out_bach,size_t amoung)
    {
        core::array<core::array<float>> in,out;
        in.resize(amoung);
        out.resize(amoung);

        for(size_t i = 0; i < amoung; i++)
        {
            in[i].resize(2);
            out[i].resize(1);
        }
        for(size_t i = 0; i < amoung; i++)
        {
            and_data(dist_data(rd),in[i],out[i]);
        }

        in_bach.push_back(in);
        out_bach.push_back(out);
    }
    void and_data(size_t data,core::array<float>& in, core::array<float>& out)
    {
        switch(data)
        {
        case 0:
                in[0] = 0 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[0] = 0 + dist_error(rd);
            break;
        case 1:
                in[0] = 0 + dist_error(rd);
                in[1] = 1 + dist_error(rd);
                out[0] = 0 + dist_error(rd);
            break;
        case 2:
                in[0] = 1 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[0] = 0 + dist_error(rd);
            break;
        case 3:
        //case 4:
        //case 5:
                in[0] = 1 + dist_error(rd);
                in[1] = 1 + dist_error(rd);
                out[0] = 1 + dist_error(rd);
            break;
        }
    }
    bool is(const T& data) const
    {
        if(data > T(1) and data < T(1) + error) return true;
        else if(core::equal(T(1),data)) return true;
        else if(core::equal(T(1) + error,data)) return true;
        else if(data > T(1)) return true;

        if(data > T(0)  and data < T(0) + error) return false;
        else if(core::equal(T(0),data)) return false;
        else if(core::equal(T(0) + error,data)) return false;

        return false;
    }

private:
    T error;
    Gate gate;
    std::random_device rd;
    std::uniform_real_distribution<float> dist_error;
    std::uniform_int_distribution<size_t> dist_data;
};
void v1_Gate_AND()
{
    BachGates<float> bach_and_1(1.0e-1f,BachGates<float>::Gate::AND);
    neuronal::Random<float> random;
    neuronal::Perceptron<float> pers1(2,1,5,4,neuronal::identity);

    core::array<core::array<float>> bach1I_1 {{0.1f,0.0f},{0.09f,1.09f},{1.1f,0.0f},{1.03f,1.01f}};
    CU_ASSERT(bach_and_1.is(bach1I_1[0][0]) == false);
    CU_ASSERT(bach_and_1.is(bach1I_1[0][1]) == false);
    CU_ASSERT(bach_and_1.is(bach1I_1[1][0]) == false);
    CU_ASSERT(bach_and_1.is(bach1I_1[1][1]) == true);


    core::array<core::array<float>> bach1I;
    core::array<core::array<float>> bach1O;
    core::array<core::array<float>> bach2I;
    core::array<core::array<float>> bach2O;
    bach_and_1.generate(bach1I,bach1O,1000);
    bach_and_1.generate(bach2I,bach2O,10);
    /*std::cout << "Data\n";
    for(size_t i = 0; i < bach1I.size(); i++)
    {
        bach1I[i].print(std::cout);
        std::cout << " --> ";
        bach1O[i].print(std::cout);
        std::cout << "\n";
    }
    std::cout << "\n\n";*/
    /*for(size_t i = 0; i < bach2I.size(); i++)
    {
        if(bach_and_1.is(bach2I[i][0]))
        {
            std::cout << bach2I[i][0] << " --> true\n";
        }
        else
        {
            std::cout << bach2I[i][0] << " --> false\n";
        }

    }
    std::cout << ">>>>>>>>>\n";*/

    pers1.feedforward(bach1I);

    //random.next();

    neuronal::Backp<float> back1(bach1I,bach1O,pers1,neuronal::identity_D,1.21e-7,1,1.0e-2);
    back1.training(100,100,std::cout);

    size_t back1_fails = 0;
    for(size_t i = 0; i < bach2I.size(); i++)
    {
        pers1.feedforward(bach2I[i]);
        if(bach_and_1.is(bach2I[i][0]) and bach_and_1.is(bach2I[i][1]))
        {
            if(pers1.back().outputs[0][0] < 1)
            {
                bach2I[i].print(std::cout);
                std::cout << " --> ";
                std::cout << pers1.back().outputs[0][0] << " Fail\n";
                back1_fails++;
            }
        }
        else
        {
            if(pers1.back().outputs[0][0] > 1)
            {
                bach2I[i].print(std::cout);
                std::cout << " --> ";
                std::cout << pers1.back().outputs[0][0] << " Fail\n";
                back1_fails++;
            }
        }
    }
    if(back1_fails > 0) std::cout << "Fallos totales : " << back1_fails << " de " << bach2I.size() << " : " << float(100) * float(back1_fails)/float(bach2I.size()) << "%\n";
    if(back1_fails <= 1)
    {
        for(size_t i = 0; i < bach2I.size(); i++)
        {
            bach2I[i].print(std::cout);
            std::cout << " --> ";
            std::cout << pers1.back().outputs[0][0] << "\n";
        }
    }
    std::cout << "\n\n";
    CU_ASSERT(back1_fails == 0);
}





void v0_developing()
{

}
