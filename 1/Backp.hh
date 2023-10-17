#ifndef OCTETOS_NEURONAL_V1_BACKP_HH
#define OCTETOS_NEURONAL_V1_BACKP_HH


namespace oct::neu::v0
{

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Backp
    {
    public://constructors
        Backp() = default;
        Backp(core::array<core::array<I>>& ins,core::array<core::array<O>>& outs,Perceptron<I,W,O,B>& p,O (*d)(I)) : inputs(ins),outputs(outs),perceptro(p)
        {
        }

    public://fuciones miembros
        void training()
        {

        }
        O cost(size_t d)
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
        O cost()
        {
            O S = 0;
            for(size_t d = 0; d < inputs.size(); d++)
            {
                S = cost(d);
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
                for(size_t i = 0; i < perceptro.layer(l).size(); i++)
                {
                    e += perceptro.layer(l + 1)[i].weights[i][i] * error(l + 1);
                }
            }
            e *= (*dereivation)(perceptro.layer(l).outputs[n][0]);

            return e;
        }

    private:
        core::array<core::array<I>>& inputs;
        core::array<core::array<O>>& outputs;
        Perceptron<I,W,O,B>& perceptro;
        O (*dereivation)(I);
    };
}


#endif // OCTETOS_NEURONAL_V1_HH
