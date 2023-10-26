
#include <iostream>

#include "v0.hh"

#include <cmath>

#include <neuronal/1/Perceptron.hh>
#include <neuronal/1/Backp.hh>
#include <limits>

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

void fill_bach_2(core::array<core::array<float>>& in,core::array<core::array<float>>& out,size_t size)
{
    size_t sub = 5;
    size = size/sub;
    size = size*sub;
    in.resize(size);
    out.resize(size);

    float actual = 0,delta = 1.0e-2f,err = 1.0e-3f;

    std::random_device rd;
    std::uniform_real_distribution<float> dist(err,1.0);

    for(size_t i = 0; i < size + sub;i += sub)
    {
        if(i == size) break;
        //std::cout << "\tIndice  : " << i << " de " << size <<  "\n";
        //correct
        in[i + 0].resize(6);
        out[i + 0].resize(2);
        in[i + 0][0] = actual;
        in[i + 0][1] = std::sin(actual);
        actual += delta;
        in[i + 0][2] = actual;
        in[i + 0][3] = std::sin(actual);
        actual += delta;
        in[i + 0][4] = actual;
        in[i + 0][5] = std::sin(actual);
        out[i + 0][0] = 1.0f;
        out[i + 0][1] = 0.0f;

        //fail
        in[i + 1].resize(6);
        out[i + 1].resize(2);
        in[i + 1][0] = actual;
        in[i + 1][1] = std::sin(actual) + dist(rd);
        actual += delta;
        in[i + 1][2] = actual;
        in[i + 1][3] = std::sin(actual)  + dist(rd);
        actual += delta;
        in[i + 1][4] = actual;
        in[i + 1][5] = std::sin(actual)  + dist(rd);
        out[i + 1][0] = 0.0f;
        out[i + 1][1] = 1.0f;

        //fail
        in[i + 2].resize(6);
        out[i + 2].resize(2);
        in[i + 2][0] = actual;
        in[i + 2][1] = std::sin(actual) - dist(rd);
        actual += delta;
        in[i + 2][2] = actual;
        in[i + 2][3] = std::sin(actual)  - dist(rd);
        actual += delta;
        in[i + 2][4] = actual;
        in[i + 2][5] = std::sin(actual)  - dist(rd);
        out[i + 2][0] = 0.0f;
        out[i + 2][1] = 1.0f;

        //fail
        in[i + 3].resize(6);
        out[i + 3].resize(2);
        in[i + 3][0] = actual;
        in[i + 3][1] = std::sin(actual) + (2 * dist(rd));
        actual += delta;
        in[i + 3][2] = actual;
        in[i + 3][3] = std::sin(actual)  + (2 * dist(rd));
        actual += delta;
        in[i + 3][4] = actual;
        in[i + 3][5] = std::sin(actual)  + (2 * dist(rd));
        out[i + 3][0] = 0.0f;
        out[i + 3][1] = 1.0f;

        //fail
        in[i + 4].resize(6);
        out[i + 4].resize(2);
        in[i + 4][0] = actual;
        in[i + 4][1] = std::sin(actual) - (2 * dist(rd));
        actual += delta;
        in[i + 4][2] = actual;
        in[i + 4][3] = std::sin(actual)  - (2 * dist(rd));
        actual += delta;
        in[i + 4][4] = actual;
        in[i + 4][5] = std::sin(actual)  - (2 * dist(rd));
        out[i + 4][0] = 0.0f;
        out[i + 4][1] = 1.0f;
    }

}

template<core::number T> class FunctionBach
{
public:
    FunctionBach(T e,T c) : error(e), cube(c), dist_coordendades(0,c),dist_tang(std::numeric_limits<T>::epsilon(),1.0f - std::numeric_limits<T>::epsilon()),dist_coordendades_delta(std::numeric_limits<T>::epsilon(),c / 10),outputs(2)
    {
    }

    /**
    *\brief Crea un lote de datos para la linea en 2D
    */
    core::array<core::array<T>> in_line(size_t amoung,size_t sub)
    {
        core::array<core::array<T>> bach;
        bach.resize(amoung);
        for(size_t i = 0; i < amoung; i++)
        {
            bach[i].resize(2 * sub); // dos cordanada(2D) por cada sub
        }

        T m = dist_tang(rd);
        T b = dist_coordendades(rd);
        T x;

        for(size_t i = 0; i < amoung; i++)
        {
            x = dist_coordendades(rd);
            for(size_t j = 0; j < sub; j++)
            {
                x += dist_coordendades_delta(rd);
                bach[i][(j * 2) + 0] = x; //corrdenada x
                //std::cout << (j * 2) + 0 << ",";
                bach[i][(j * 2) + 1] = (m * x) + b; //corrdenada y
                //std::cout << (j * 2) + 1 << ",";
            }
            //std::cout << "\n";
        }

        return bach;
    }
    /**
    *\brief Crea un lote de datos para la salida de la red
    */
    core::array<core::array<T>> out_line(size_t amoung,size_t sub)
    {
        core::array<core::array<T>> bach;
        bach.resize(amoung);
        for(size_t i = 0; i < amoung; i++)
        {
            bach[i].resize(outputs); // dos cordanada(2D) por cada sub
            for(size_t j = 0; j < outputs; j++)
            {
                bach[i][j] = 0; // dos cordanada(2D) por cada sub
            }
        }



        for(size_t i = 0; i < amoung; i++)
        {
            for(size_t j = 0; j < sub; j++)
            {
                bach[i][0] = T(1); //corrdenada x
            }
        }

        return bach;
    }

    void generate(core::array<core::array<float>>& ins, core::array<core::array<float>>& outs,size_t amoung,size_t sub)
    {
        //
        size_t miniback = 5;
        size_t sets = 0;
        core::array<core::array<T>> mini_data_in,mini_data_out;

        if(amoung < sets + miniback) return;
        mini_data_in = in_line(miniback,sub);
        mini_data_out = out_line(miniback,sub);
        ins.push_back(mini_data_in);
        outs.push_back(mini_data_out);
        sets += miniback;


        if(amoung < sets + miniback) return;
        mini_data_in = in_line(miniback,sub);
        mini_data_out = out_line(miniback,sub);
        ins.push_back(mini_data_in);
        outs.push_back(mini_data_out);
        sets += miniback;


        if(amoung < sets + miniback) return;
        mini_data_in = in_line(miniback,sub);
        mini_data_out = out_line(miniback,sub);
        ins.push_back(mini_data_in);
        outs.push_back(mini_data_out);
        sets += miniback;


        if(amoung < sets + miniback) return;
        mini_data_in = in_line(miniback,sub);
        mini_data_out = out_line(miniback,sub);
        ins.push_back(mini_data_in);
        outs.push_back(mini_data_out);
        sets += miniback;


        if(amoung < sets + miniback) return;
        mini_data_in = in_line(miniback,sub);
        mini_data_out = out_line(miniback,sub);
        ins.push_back(mini_data_in);
        outs.push_back(mini_data_out);
        sets += miniback;

    }

private:
    T error;
    T cube;
    std::random_device rd;
    std::uniform_real_distribution<float> dist_coordendades;
    std::uniform_real_distribution<float> dist_tang;
    std::uniform_real_distribution<float> dist_coordendades_delta;
    size_t outputs;
};
void v0_developing()
{
    core::array<core::array<float>> bach1I {{0.0f,0.0f},{0.0f,1.0f},{1.0f,0.0f},{1.0f,1.0f}};
    //bach1I[3].print(std::cout);
    core::array<core::array<float>> bach1O {{0.0f},{0.0f},{0.0f},{1.0f}};
    neuronal::Perceptron<float> pers1(2,1,2,2,fun1);
    pers1.feedforward(bach1I);

    neuronal::Backp<float> back1(bach1I,bach1O,pers1,dev1,1.0e-4,1.0e-2,1.0e-2);
    float e1;
    for(size_t i = 0; i < 1000; i++)
    {
        e1 = back1.error();
        std::cout << "Error " << i  << " : " << e1 << "\n";
        if(e1 < 1.0e-3f) break;
        back1.iteration();
    }



    /*
    core::array<core::array<float>> bach2I;
    core::array<core::array<float>> bach2O;
    FunctionBach<float> fill_bach_3(1.0e-3,1000);

    fill_bach_3.generate(bach2I,bach2O,100,3);
    for(size_t i = 0; i < bach2I.size(); i++)
    {
        bach2I[i].print(std::cout);
        std::cout << " -- " ;
        bach2O[i].print(std::cout);
        std::cout << "\n" ;
    }
    neuronal::Perceptron<float> pers2(6,2,5,10,fun1,1.0e-1f,0.0f);
    pers2.feedforward(bach2I);

    neuronal::Backp<float> back2(bach2I,bach2O,pers2,dev1,1.0e-4);

    //std::cout << "Error : " << e1 << "\n";
    //back2.iteration()
    std::cout << "\n";

    float e1;//= back2.cost();
    for(size_t i = 0; i < 1000; i++)
    {
        e1 = back2.error();
        std::cout << "Error " << i  << " : " << e1 << "\n";
        if(e1 < 1.0e-3f) break;
        back2.iteration();
    }*/


    /*core::array<float> in2(6);
    in2[0] = 0.09;
    in2[1] = std::sin(in2[0]);
    in2[2] = in2[0] + 0.01f;
    in2[3] = std::sin(in2[2]);
    in2[4] = in2[2] + 0.01f;
    in2[5] = std::sin(in2[4]);
    pers2.feedforward(in2);
    pers2.back().outputs.print(std::cout);*/

}



