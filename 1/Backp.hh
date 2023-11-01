#ifndef OCTETOS_NEURONAL_V1_BACKP_HH
#define OCTETOS_NEURONAL_V1_BACKP_HH


#include <random>
#include <thread>
#include <chrono>

#include "Perceptron.hh"


namespace oct::neu::v0
{

    template<core::number W> class Random
    {
    public:
        Random() : gen{rd()},dist(1.0f,0.25f)
        {
        }
        W next()
        {
            return dist(gen);
        }
    private:
    std::random_device rd;
    std::mt19937 gen;
    std::normal_distribution<float> dist;

    };

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Backp
    {
    public://constructors
        Backp() = default;
        Backp(const core::array<core::array<I>>& ins,const core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I),W r,O err) : inputs(ins),outputs(outs),perceptro(p),derivaties(p.size()),ratio(r),errors(p.size()),max_err(err)
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
        Backp(const core::array<core::array<I>>& ins,const core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I),W r,W init_weights,O err) : inputs(ins),outputs(outs),perceptro(p),derivaties(p.size()),ratio(r),errors(p.size()),max_err(err)
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
            for(size_t i = 0; i < p.size(); i++)
            {
                for(size_t j = 0; j < p[i].height; j++)
                {
                    for(size_t k = 0; k < p[i].weights.rows(); k++)
                    {
                        p[i].weights[j][k] = init_weights;
                    }
                }
            }
        }
        Backp(const core::array<core::array<I>>& ins,const core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I),W r,Random<W>& rand,O err) : inputs(ins),outputs(outs),perceptro(p),derivaties(p.size()),ratio(r),errors(p.size()),max_err(err)
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
            for(size_t i = 0; i < p.size(); i++)
            {
                for(size_t j = 0; j < p[i].height; j++)
                {
                    for(size_t k = 0; k < p[i].weights.rows(); k++)
                    {
                        p[i].weights[j][k] = rand.next();
                    }
                }
            }
        }

    public://fuciones miembros
        void iteration()
        {
            //using namespace std::chrono_literals;
            //std::this_thread::sleep_for(1ms);
            for(size_t d = 0; d < inputs.size() ; d++)
            {
                //perceptro.feedforward(inputs[d]);
                //perceptro[perceptro.size() - 2].weights.print(std::cout);

                for(size_t o = 0; o < perceptro.back().height; o++)
                {
                    perceptro.feedforward(inputs[d]);

                    errors.back().back() = dCdf(d,o);
                    for(size_t w = 0; w < perceptro.back().weights.columns(); w++)
                    {
                        perceptro.back().weights[o][w] -= dfdw(perceptro.size() - 1,o,w) * errors.back().back() * ratio;
                        //std::cout << "dfdw = " << dfdw(errors.size() - 1,o,w) << "\terror = " << errors.back().back() << "\n";
                    }
                    /*errors.back().back() = 0;
                    for(size_t n = 0; n < perceptro.back().height; n++)
                    {
                        errors.back().back() += errors.back()[n];
                    }
                    errors.back().back() /= O(errors.back().size());*/
                    //std::cout << "\terror = " << errors.back().back() << "\n";

                    for(int l = perceptro.size() - 2; l > 0 ; l--)
                    {
                        for(size_t n = 0; n < perceptro[l].height; n++)
                        {
                            for(size_t w = 0; w < perceptro[l].weights.columns(); w++)
                            {
                                //std::cout << "Layer : " << l << "\t" << "dfdw = " << dfdw(l,n,w) << "\terror = " << get_error_for(l) << "\n";
                                perceptro[l].weights[n][w] -= dfdw(l,n,w) * get_error_for(l) * ratio;
                                errors[l][n] += dfdw(l,n,w) * get_error_for(l);
                            }
                        }
                        errors[l].back() = 0;
                        for(size_t n = 0; n < perceptro[l].height; n++)
                        {
                            errors[l].back() += errors[l][n];
                        }
                        errors[l].back() /= O(errors[l].size());
                    }
                    //capa mde entradas
                    for(size_t n = 0; n < perceptro.front().height; n++)
                    {
                        perceptro.front().weights[n][0] -= dfdo(0,n,0) * perceptro.front().weights[n][0] * get_error_for(0) * ratio;
                    }
                }
                //

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
            E /= O(outputs.size() * 2);

            return E;
        }
        O error()
        {
            //std::cout << "Error \n";
            O S = 0;
            for(size_t d = 0; d < inputs.size(); d++)
            {
                S += error(d);
                //std::cout << "\t" << S << "\n";
            }
            S /= O(inputs.size());
            //std::cout << "\tTotal : " << S << "\n";

            return S;
        }


        /*
        *
        */
        O dCdf(size_t d,size_t o)
        {
            return (perceptro.back().outputs[o][0] - outputs[d][o]) * O(2);
        }
        O dfdo(size_t l, size_t n,size_t w)
        {
            return (*derivaties[l])(perceptro[l].outputs[n][0]);
        }
        O dodw(size_t l, size_t n,size_t w)
        {
            return (*derivaties[l - 1])(perceptro[l - 1].outputs[w][0]);
        }
        O dfdw(size_t l, size_t n,size_t w)
        {
            return dfdo(l,n,w) * dodw(l,n,w);
        }


        O get_error_for(size_t l) const
        {
            return errors[l + 1].back();
        }

    public:
        void training(size_t epoch, size_t it)
        {
            for(size_t e = 0; e < epoch; e++)
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(5ms);
                for(size_t i = 0; i < it; i++)
                {
                    iteration();
                }
            }
        }

        void training(size_t epoch, size_t it,std::ostream& out)
        {
            O actual_error;
            for(size_t e = 0; e < epoch; e++)
            {
                actual_error = error();
                out << "Epoca : " << e;
                out << "\tError : " << error() << "\n";
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(5ms);
                if(max_err > actual_error)
                {
                    out << "Max error : " << actual_error << "\n";
                    return;
                }
                for(size_t i = 0; i < it; i++)
                {
                    iteration();
                }
            }
        }

    private:
        const core::array<core::array<I>>& inputs;
        const core::array<core::array<O>>& outputs;
        Perceptron<I,W,O,B>& perceptro;
        core::array<O (*)(I)> derivaties;
        W ratio;
        core::array<core::array<O>> errors;
        O max_err;

    };
}


#endif // OCTETOS_NEURONAL_V1_HH
