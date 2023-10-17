#ifndef OCTETOS_NEURONAL_V1_BACKP_HH
#define OCTETOS_NEURONAL_V1_BACKP_HH


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
            for(int l = perceptro.size() - 1; l >= 0 ; l--)
            {
                for(size_t n = 0; n < perceptro[l].height; n++)
                {
                    for(size_t w = 0; w < perceptro[l].weights.columns(); w++)
                    {
                        perceptro[l].weights[n][w] -= dEdw(l,n) * ratio;
                    }
                }
            }
        }
        O error(size_t d)
        {
            O E = 0, e;
            perceptro.spread(inputs[d]);
            for(size_t o = 0; o < outputs[d].size(); o++)
            {
                e = outputs[d][o] - perceptro.output().outputs[o][0];
                e *= O(2);
                E += e;
            }
            E /= O(outputs.size());

            return E;
        }
        O error()
        {
            O S = 0;
            for(size_t d = 0; d < inputs.size(); d++)
            {
                S = error(d);
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

        O dEdw(size_t l, size_t n)
        {
            return error(l,n) * (*perceptro[l].activation)(perceptro[l].outputs[n][0]);
        }

    private:
        core::array<core::array<I>>& inputs;
        core::array<core::array<O>>& outputs;
        Perceptron<I,W,O,B>& perceptro;
        O (*derivation)(I);
        W ratio;
    };
}


#endif // OCTETOS_NEURONAL_V1_HH
