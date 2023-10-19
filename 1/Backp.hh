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
        Backp(const core::array<core::array<I>>& ins,const core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I),W r) : inputs(ins),outputs(outs),perceptro(p),derivaties(p.size()),ratio(r),errors(p.size())
        {
            for(size_t i = 0; i < p.size(); i++)
            {
                errors[i].resize(p[i].height + 1);
                for(size_t j = 0; j < errors[i].size(); j++)
                {
                    errors[i][j] = 0;
                }
            }
            for(size_t i = 0; i < derivaties.size(); i++)
            {
                derivaties[i] = d;
            }
        }

    public://fuciones miembros
        void iteration()
        {

            for(size_t d = 0; d < inputs.size() ; d++)
            {
                perceptro.feedforward(inputs[d]);

                for(size_t o = 0; o < perceptro.back().height; o++)
                {
                    //perceptro.feedforward(inputs[d]);

                    errors[errors.size() - 1][o] = dEdo(d,o);
                    for(size_t w = 0; w < perceptro.back().weights.columns(); w++)
                    {
                        perceptro.back().weights[o][w] += dEdw(errors.size() - 1,o,errors[errors.size() - 1][0]) * ratio;
                    }
                    errors.back().back() = 0;
                    for(size_t n = 0; n < perceptro.back().height; n++)
                    {
                        errors.back().back() += errors.back()[n];
                    }
                    errors.back().back() /= O(errors.back().size());
                }

                for(int l = perceptro.size() - 2; l >= 0 ; l--)
                {
                    for(size_t n = 0; n < perceptro[l].height; n++)
                    {
                        for(size_t w = 0; w < perceptro[l].weights.columns(); w++)
                        {
                            perceptro[l].weights[n][w] += dEdw(l,n,get_error_in(l,n)) * ratio;
                        }
                    }
                    errors[l].back() = 0;
                    for(size_t n = 0; n < perceptro[l].height; n++)
                    {
                        errors[l].back() += errors[l][n];
                    }
                    errors[l].back() /= O(errors[l].size());
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

        O dEdo(size_t d,size_t o)
        {
            O e = std::abs(perceptro.back().outputs[o][0] - outputs[d][o]);
            e *= O(2);
            //std::cout << "dEdo : " << e << "\n";

            return e;
        }

        O dodw(size_t l, size_t n,O error)
        {
            //std::cout << "dOdf : " << (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0])) << "\n";
            O E = (*derivaties[l])(perceptro[l].outputs[n][0]);
            O ws = 0;
            if(l == perceptro.size() - 1)
            {
                for(size_t i = 0; i < perceptro[l].height; i++)
                {
                    ws += perceptro[l].weights[i][n];
                }
            }
            else
            {
                for(size_t i = 0; i < perceptro[l + 1].height; i++)
                {
                    ws += perceptro[l + 1].weights[i][n];
                }
            }

            return E * ws * error;
        }
        O dEdw(size_t l, size_t n,O error)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";
            return error * (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }

        O get_error_in(size_t l, size_t n) const
        {
            if(l == perceptro.size() - 1)
            {
                return errors[l].back();
            }
            else if(l == perceptro.size() - 2)
            {
                return errors[l].back();
            }
            else
            {
                return errors[l].back();
            }

            return 0;
        }


    private:
        const core::array<core::array<I>>& inputs;
        const core::array<core::array<O>>& outputs;
        Perceptron<I,W,O,B>& perceptro;
        core::array<O (*)(I)> derivaties;
        W ratio;
        core::array<core::array<O>> errors;

    };
}


#endif // OCTETOS_NEURONAL_V1_HH
