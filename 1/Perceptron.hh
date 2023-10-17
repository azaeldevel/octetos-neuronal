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
        size_t height;

        Layer() = default;
        Layer(size_t inputs,W init_weights,B init_bias) : height(1)//perceptron simple
        {
            weights.resize(height,inputs);
            bias.resize(height,1);
            outputs.resize(height,1);
            for(size_t i = 0; i < weights.columns(); i++)
            {
                weights[0][i] = init_weights;
            }
            bias[0][0] = init_bias;
            outputs[0][0] = 0;
        }
        Layer(size_t inputs,W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n)) : height(1)//perceptron simple
        {
            weights.resize(height,inputs);
            bias.resize(height,1);
            outputs.resize(height,1);
            for(size_t i = 0; i < weights.rows(); i++)
            {
                weights[0][i] = (*init_weights)(1,i);
            }
            bias[0][0] = (*init_bias)(0);
            outputs[0][0] = 0;
        }
        Layer(size_t inputs,size_t h,W init_weights,B init_bias) : height(h)//perceptron simple
        {
            weights.resize(height,inputs);
            bias.resize(height,1);
            outputs.resize(height,1);
            for(size_t n = 0; n < height; n++)
            {
                for(size_t i = 0; i < inputs; i++)
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

    /**
    *\brief Crea un Perceptron simple un multi-capa
    **/
    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Perceptron
    {
    public://contructores
        /**
        *\brief Contrulle un perceptron simple
        *
        **/
        Perceptron(size_t ins,O (*activation)(I),O (*dereivation)(I),W init_weights,B init_bias) : layers(1),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);
        }
        /**
        *\brief Contrulle un perceptron simple
        *
        **/
        Perceptron(size_t ins,O (*activation)(I),O (*dereivation)(I),W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n)) : layers(1),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);
        }

        /**
        *\brief Contrulle un perceptron multi-capa
        *
        **/
        Perceptron(size_t ins,size_t outs,size_t height,size_t l,W init_weights,B init_bias) : layers(l),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);

            for(size_t l = 1; l < layers.size(); l++)
            {
                layers[l] = Layer<I,W,O,B>(layers[l - 1].size(),height,init_weights,init_bias);
            }

            output = Layer<I,W,O,B>(layers[layers.size() - 2].size(),outs,init_weights,init_bias);
        }
        /**
        *\brief Contrulle un perceptron multi-capa
        *
        **/
        Perceptron(size_t ins,size_t outs,size_t height,size_t l,O (*activation)(I),O (*dereivation)(I),W init_weights,B init_bias) : layers(l),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);

            for(size_t l = 1; l < layers.size() - 1; l++)
            {
                layers[l] = Layer<I,W,O,B>(layers[l - 1].height,height,init_weights,init_bias);
            }

            output() = Layer<I,W,O,B>(layers[layers.size() - 2].height,outs,init_weights,init_bias);
        }

        Perceptron(core::array<Model<I,W,O>> model);

    public:
        void spread(core::array<I>& ins)
        {
            const numbers::matrix<I> inm(ins.size(),1,(I*)(ins));
            //std::cout << "\n";
            //inm.print(std::cout);
            //std::cout << "\n";
            //layers[0].weights.print(std::cout);
            //std::cout << "\n";
            //layers[0].bias.print(std::cout);
            //std::cout << "\n";
            layers[0].outputs = layers[0].weights * inm + layers[0].bias;
            //layers[0].outputs.print(std::cout);
            //std::cout << "\n";

            for(size_t layer = 1; layer < layers.size(); layer++)
            {
                layers[layer].outputs = layers[layer].weights * layers[layer - 1].outputs + layers[layer].bias;
                layers[layer].outputs.print(std::cout);
            }
        }
        void spread(core::array<core::array<I>>& ins)
        {
            for(size_t data = 0; data < ins.size(); data++)
            {
                spread(ins[data]);
            }
        }

        Layer<I,W,O,B>& output()
        {
            return layers[layers.size() - 1];
        }

    private:
        core::array<Layer<I,W,O,B>> layers;
        size_t inputs;

    protected:
    public:

    };
}


#endif // PERCEPTRON_HH_INCLUDED
