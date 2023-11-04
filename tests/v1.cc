
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
        OR,
        ORAND,
        ANDOR
    };

public:
    BachGates(T e,Gate g) : error(e),gate(g),dist_error(0,e),dist_data(0,3),gen(rd())
    {
    }

    void generate(core::array<core::array<float>>& in_bach,core::array<core::array<float>>& out_bach,size_t amoung)
    {
        switch(gate)
        {
        case Gate::AND:
            and_gate(in_bach,out_bach,amoung);
            break;
        case Gate::OR:
            or_gate(in_bach,out_bach,amoung);
            break;
        case Gate::ORAND:
            or_and_gate(in_bach,out_bach,amoung);
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
    void or_gate(core::array<core::array<float>>& in_bach,core::array<core::array<float>>& out_bach,size_t amoung)
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
            or_data(dist_data(rd),in[i],out[i]);
        }

        in_bach.push_back(in);
        out_bach.push_back(out);
    }
    void or_and_gate(core::array<core::array<float>>& in_bach,core::array<core::array<float>>& out_bach,size_t amoung)
    {
        core::array<core::array<float>> in,out;
        in.resize(amoung);
        out.resize(amoung);
        std::bernoulli_distribution benulli(0.5);

        for(size_t i = 0; i < amoung; i++)
        {
            in[i].resize(2);
            out[i].resize(2);
            out[i][0] = 0;
            out[i][1] = 0;
        }
        for(size_t i = 0; i < amoung; i++)
        {
            if(benulli(gen)) or_data(dist_data(rd),in[i],out[i],0);
            else and_data(dist_data(rd),in[i],out[i],1);
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

    void or_data(size_t data,core::array<float>& in, core::array<float>& out)
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
                out[0] = 1 + dist_error(rd);
            break;
        case 2:
                in[0] = 1 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[0] = 1 + dist_error(rd);
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

    void and_data(size_t data,core::array<float>& in, core::array<float>& out,size_t index_out)
    {
        switch(data)
        {
        case 0:
                in[0] = 0 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[index_out] = 0 + dist_error(rd);
            break;
        case 1:
                in[0] = 0 + dist_error(rd);
                in[1] = 1 + dist_error(rd);
                out[index_out] = 0 + dist_error(rd);
            break;
        case 2:
                in[0] = 1 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[index_out] = 0 + dist_error(rd);
            break;
        case 3:
        //case 4:
        //case 5:
                in[0] = 1 + dist_error(rd);
                in[1] = 1 + dist_error(rd);
                out[index_out] = 1 + dist_error(rd);
            break;
        }
    }

    void or_data(size_t data,core::array<float>& in, core::array<float>& out,size_t index_out)
    {
        switch(data)
        {
        case 0:
                in[0] = 0 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[index_out] = 0 + dist_error(rd);
            break;
        case 1:
                in[0] = 0 + dist_error(rd);
                in[1] = 1 + dist_error(rd);
                out[index_out] = 1 + dist_error(rd);
            break;
        case 2:
                in[0] = 1 + dist_error(rd);
                in[1] = 0 + dist_error(rd);
                out[index_out] = 1 + dist_error(rd);
            break;
        case 3:
        //case 4:
        //case 5:
                in[0] = 1 + dist_error(rd);
                in[1] = 1 + dist_error(rd);
                out[index_out] = 1 + dist_error(rd);
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
    std::mt19937 gen;
};
void v1_Gate_AND()
{
    BachGates<float> bach_and_1(1.0e-1f,BachGates<float>::Gate::AND);
    neuronal::Random<float> random(0.5f);
    neuronal::Perceptron<float> pers1(2,1,3,2,neuronal::identity);

    core::array<core::array<float>> bach1I;
    core::array<core::array<float>> bach1O;
    core::array<core::array<float>> bach2I;
    core::array<core::array<float>> bach2O;
    bach_and_1.generate(bach1I,bach1O,1000);
    bach_and_1.generate(bach2I,bach2O,10);

    neuronal::Backp<float> back1(bach1I,bach1O,pers1,neuronal::identity_D,1.31e-3,random,1.0e-4);
    back1.training(100,100);
    /*size_t back1_fails = 0;
    for(size_t i = 0; i < bach2I.size(); i++)
    {
        pers1.feedforward(bach2I[i]);
        {
            bach2I[i].print(std::cout);
            std::cout << " --> ";
            std::cout << pers1.back().outputs[0][0] << "\n";
        }
    }
    if(back1_fails > 0) std::cout << "Fallos totales : " << back1_fails << " de " << bach2I.size() << " : " << float(100) * float(back1_fails)/float(bach2I.size()) << "%\n";
    std::cout << "\n\n";
    CU_ASSERT(back1_fails == 0);*/

    core::array<core::array<float>> bach3I {{0.1f,0.0f},{0.09f,1.09f},{1.1f,0.0f},{1.03f,1.01f}};
    pers1.feedforward(bach3I[0]);
    CU_ASSERT(pers1.back().outputs[0][0] < 0.5f)
    pers1.feedforward(bach3I[1]);
    CU_ASSERT(pers1.back().outputs[0][0] < 0.5f)
    pers1.feedforward(bach3I[2]);
    CU_ASSERT(pers1.back().outputs[0][0] < 0.5f)
    pers1.feedforward(bach3I[3]);
    CU_ASSERT(pers1.back().outputs[0][0] > 0.5f)

}




void v0_developing()
{

}
