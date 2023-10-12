#ifndef PERCEPTRON_HH_INCLUDED
#define PERCEPTRON_HH_INCLUDED


#include <core/3/numbers.hh>
#include <core/3/array.hh>
#include <numbers/0/common.hh>
#include <numbers/0/matrix.hh>

namespace oct::neu::v0
{
    namespace numbers = oct::nums::v0;
    namespace core = oct::core::v3;

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> struct Model
    {
        size_t inputs;
        size_t outputs;
        size_t height;//cantidad de neuronas en el layer
        O (*activation)(I);
        O (*dereivation)(I);
    };

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> struct Layer
    {
        numbers::matrix<W> weights;
        numbers::matrix<B> bias;
        numbers::matrix<O> outputs;

        Layer() = default;
        Layer(size_t inputs,W init_weights,B init_bias)//perceptron simple
        {
            weights.resize(1,inputs);
            bias.resize(1,1);
            outputs.resize(1,1);
            for(size_t i = 0; i < weights.rows(); i++)
            {
                weights[0][i] = init_weights;
            }
            bias[0][0] = init_bias;
            outputs[0][0] = 0;
        }
        Layer(size_t inputs,W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n))//perceptron simple
        {
            weights.resize(1,inputs);
            bias.resize(1,1);
            outputs.resize(1,1);
            for(size_t i = 0; i < weights.rows(); i++)
            {
                weights[0][i] = (*init_weights)(1,i);
            }
            bias[0][0] = (*init_bias)(0);
            outputs[0][0] = 0;
        }
        Layer(size_t inputs,size_t height,W init_weights,B init_bias)//perceptron simple
        {
            weights.resize(height,inputs);
            bias.resize(height,1);
            outputs.resize(height,1);
            for(size_t n = 0; n < weights.rows(); n++)
            {
                for(size_t i = 0; i < weights[n].size(); i++)
                {
                    weights[n][i] = init_weights;
                }
                bias[n][0] = init_bias;
                outputs[n][0] = 0;
            }
        }
        Layer(Model<I,W,O,B> const& m)//perceptron multi-capa
        {
        }
    };

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Perceptron
    {
    public:
        Perceptron(size_t ins,O (*activation)(I),O (*dereivation)(I),W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n)) : layers(1),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);
        }
        Perceptron(size_t ins,O (*activation)(I),O (*dereivation)(I),W init_weights,B init_bias) : layers(1),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);
        }
        Perceptron(size_t ins,size_t outs,size_t height,O (*activation)(I),O (*dereivation)(I),W init_weights,B init_bias)
        {

        }
        Perceptron(core::array<Model<I,W,O>> model);

    public:
        void spread(core::array<core::array<I>> in,core::array<core::array<O>> out)
        {

        }

    private:
        core::array<Layer<I,W,O,B>> layers;
        size_t inputs;

    protected:
    public:

    };
}


#endif // PERCEPTRON_HH_INCLUDED
