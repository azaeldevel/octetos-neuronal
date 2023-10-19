#ifndef OCTETOS_NEURONAL_V1_BACKP_HH
#define OCTETOS_NEURONAL_V1_BACKP_HH


#include <random>

#include "Perceptron.hh"


namespace oct::neu::v0
{

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Backp
    {
    public://constructors
        Backp() = default;
        Backp(const core::array<core::array<I>>& ins,const core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I),W r) : inputs(ins),outputs(outs),perceptro(p),derivation(d),ratio(r),errors(p.size())
        {
            for(size_t i = 0; i < p.size(); i++)
            {
                //errors[i].resize(p[i].height + 1; i++);
            }
        }

    public://fuciones miembros
        void iteration()
        {

            for(size_t o = 0; o < perceptro.back().height; o++)
            {
                //perceptro.feedforward(inputs[d]);

                for(size_t d = 0; d < inputs.size() ; d++)
                {
                    perceptro.feedforward(inputs[d]);

                    for(size_t n = 0; n < perceptro.back().height; n++)
                    {
                        for(size_t w = 0; w < perceptro.back().weights.columns(); w++)
                        {
                            //std::cout << "dEdw : " << dEdw(l,n,d) << "\n";

                                //perceptro.back().weights[n][w] -= dEdw(perceptro.size() - 1,n,d,o) * ratio;
                        }
                    }
                }
            }
        }
        O error(size_t d)
        {
            O E = 0, e;
            perceptro.feedforward(inputs[d]);
            for(size_t o = 0; o < outputs[d].size(); o++)
            {
                e = outputs[d][o] - perceptro.back().outputs[o][0];
                E += std::pow(e,O(2));
            }
            E /= O(outputs.size());

            return E;
        }
        O error()
        {
            O S = 0;
            for(size_t d = 0; d < inputs.size(); d++)
            {
                S += error(d);
            }
            S /= O(inputs.size());

            return S;
        }




        /*O dEdw(size_t l, size_t n)
        {
            return error(l,n) * (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }*/

        O dEdo(size_t d,size_t o)
        {
            O e = std::abs(perceptro.output().outputs[o][0] - outputs[d][o]);
            e *= O(2);
            //std::cout << "dEdo : " << e << "\n";

            return e;
        }

        O dodw(size_t l, size_t n)
        {
            //std::cout << "dOdf : " << (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0])) << "\n";
            return (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0]));
        }


        O dodf(size_t l, size_t n,O error)
        {
            //std::cout << "dOdf : " << (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0])) << "\n";
            O E = (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0]));
            O e = 0;
            if(l == perceptro.size() - 1)
            {
                for(size_t i = 0; i < perceptro[l].height; i++)
                {
                    e += perceptro[l].weights[i][n];
                }
            }
            else
            {
                for(size_t i = 0; i < perceptro[l + 1].height; i++)
                {
                    e += perceptro[l + 1].weights[i][n];
                }
            }

            return E * e * error;
        }
        O dEdw(size_t l, size_t n,O error)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";
            return error * (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }


    private:
        const core::array<core::array<I>>& inputs;
        const core::array<core::array<O>>& outputs;
        Perceptron<I,W,O,B>& perceptro;
        O (*derivation)(I);
        W ratio;
        core::array<core::array<O>> errors;
    };
}


#endif // OCTETOS_NEURONAL_V1_HH
