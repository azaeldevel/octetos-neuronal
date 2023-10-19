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
        Backp(core::array<core::array<I>>& ins,core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I),W r) : inputs(ins),outputs(outs),perceptro(p),derivation(d),ratio(r)
        {
        }

    public://fuciones miembros
        void iteration()
        {
            std::random_device rd;

            for(size_t d = 0; d < inputs.size() ; d++)
            {
                perceptro.feedforward(inputs[d]);

                for(size_t o = 0; o < perceptro.output().height; o++)
                {
                    //perceptro.feedforward(inputs[d]);

                    /*for(size_t n = 0; n < perceptro.output().height; n++)
                    {
                        //for(size_t o = 0; o < perceptro.output().height; o++)
                        {
                            for(size_t w = 0; w < perceptro.output().weights.columns(); w++)
                            {
                                //std::cout << "dEdw : " << dEdw(l,n,d) << "\n";
                                perceptro.output().weights[n][w] -= dEdw(perceptro.size() - 1,n,d,o) * ratio;
                            }
                        }
                    }*/
                    //perceptro.feedforward(inputs[d]);

                    for(int l = perceptro.size() - 1; l >= 0 ; l--)
                    {
                        for(size_t n = 0; n < perceptro[l].height; n++)
                        {

                            std::uniform_int_distribution<int> dist(0,perceptro[l].weights.columns() - 1);
                            //for(size_t o = 0; o < perceptro.output().height; o++)
                            {
                                //for(size_t w = 0; w < perceptro[l].weights.columns(); w++)
                                {
                                    //std::cout << "\tdEdw : " << dEdw(l,n,d,o) << "\n";
                                    size_t w = dist(rd);
                                    perceptro[l].weights[n][w] -= E(l,n,d,o,w) * ratio;
                                }
                            }
                        }
                    }

                    //perceptro.feedforward(inputs[d]);
                }
            }
        }
        O error(size_t d)
        {
            O E = 0, e;
            perceptro.feedforward(inputs[d]);
            for(size_t o = 0; o < outputs[d].size(); o++)
            {
                e = outputs[d][o] - perceptro.output().outputs[o][0];
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
        O error(size_t l, size_t n)
        {
            O e = 0;
            if(l == perceptro.size() - 1) return error();
            else if(l < perceptro.size() - 1)
            {
                for(size_t i = 0; i < perceptro[l + 1].height; i++)
                {
                    e += perceptro[l + 1].weights[i][i] * error(l + 1);
                }
            }
            e *= (*derivation)(perceptro[l].outputs[n][0]);

            return e;
        }

        /*O dEdw(size_t l, size_t n)
        {
            return error(l,n) * (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }*/

        O dEdo(size_t d,size_t o)
        {
            O e = perceptro.output().outputs[o][0] - outputs[d][o];
            e *= O(2);
            //std::cout << "dEdo : " << e << "\n";

            return e;
        }
        O dodf(size_t l, size_t n)
        {
            //std::cout << "dOdf : " << (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0])) << "\n";
            return (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0]));
        }
        O dfdw(size_t l, size_t n)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";
            return (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }
        O dEdw(size_t l, size_t n,size_t d,size_t o)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";

            return dEdo(d,o) * dodf(l,n) * dfdw(l,n);
        }
        O E(size_t l, size_t n,size_t d,size_t o,size_t w)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";
            O e = 0;
            if(l == perceptro.size() - 1)
            {
                for(size_t i = 0; i < perceptro[l].height; i++)
                {
                    e += dEdw(l,n,d,o) * perceptro[l].weights[i][w];
                }
                e /= O(perceptro[l].height);
            }
            else
            {
                for(size_t i = 0; i < perceptro[l + 1].height; i++)
                {
                    e += dEdw(l,n,d,o) * perceptro[l + 1].weights[i][w];
                }
                e /= O(perceptro[l + 1].height);
            }

            return e;
        }
        /*
        O dEdo(size_t d)
        {
            O E = 0, e;
            for(size_t o = 0; o < outputs[d].size(); o++)
            {
                e = perceptro.output().outputs[o][0] - outputs[d][o];
                e *= O(2);
                E += e;
            }
            E /= O(outputs.size());
            //std::cout << "dEdo : " << E << "\n";

            return E;
        }
        O dOdf(size_t l, size_t n)
        {
            //std::cout << "dOdf : " << (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0])) << "\n";
            return (*derivation)((*perceptro[l].activation)(perceptro[l].outputs[n][0]));
        }
        O dEdw(size_t l, size_t n,size_t d)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";
            return dEdo(d) * dOdf(l,n) * (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }
        O dEdw(size_t l, size_t n,size_t d,size_t w)
        {
            //std::cout << "output : " << perceptro[l].outputs[n][0] << "\n";
            //std::cout << "dEdw : " << (*perceptro[l].activation)(perceptro[l].outputs[n][0]) << "\n";
            O e = 0;
            if(l == perceptro.size() - 1)
            {
                for(size_t i = 0; i < perceptro[l].height; i++)
                {
                    e += dEdw(l,n,d) * perceptro[l].weights[i][w];
                }
                e /= O(perceptro[l].height);
            }
            else
            {
                for(size_t i = 0; i < perceptro[l + 1].height; i++)
                {
                    e += dEdw(l,n,d) * perceptro[l + 1].weights[i][w];
                }
                e /= O(perceptro[l + 1].height);
            }

            return e;
        }
        */

    private:
        core::array<core::array<I>>& inputs;
        core::array<core::array<O>>& outputs;
        Perceptron<I,W,O,B>& perceptro;
        O (*derivation)(I);
        W ratio;
    };
}


#endif // OCTETOS_NEURONAL_V1_HH
