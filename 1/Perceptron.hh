#ifndef OCTETOS_NEURONAL_V1_PERCEPTRON_HH
#define OCTETOS_NEURONAL_V1_PERCEPTRON_HH


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
        O (*activation)(I);

        Layer() = default;
        Layer(size_t inputs,O (*a)(I),W init_weights,B init_bias) : height(1),activation(a)//perceptron simple
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
        Layer(size_t inputs,O (*a)(I),W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n)) : height(1),activation(a)//perceptron simple
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
        Layer(size_t inputs,size_t h,O (*a)(I),W init_weights,B init_bias) : height(h),activation(a)//perceptron simple
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
        *\param ins Cantidad de entradas
        *\param activation Funcion de activacion
        *\param dereivation Derivada de la funcion de activacion
        *\param init_weights Valor inicial de los pesos
        *\param init_bias Valor inicial de las bias
        **/
        Perceptron(size_t ins,O (*activation)(I),W init_weights,B init_bias) : layers(1),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,activation,init_weights,init_bias);
        }
        /**
        *\brief Contrulle un perceptron simple
        *\param ins Cantidad de entradas
        *\param activation Funcion de activacion
        *\param init_weights Funcion para inicializar los pesos
        *\param init_bias Funcion para inicializar las bias
        **/
        Perceptron(size_t ins,O (*activation)(I),W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n)) : layers(1),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,activation,init_weights,init_bias);
        }

        /**
        *\brief Contrulle un perceptron multi-capa
        *\param ins Cantidad de entradas
        *\param outs Cantidad salidas
        *\param height Cantidad de neuronas por capa
        *\param l Cantidad de capas
        *\param activation Funcion de activacion
        *\param init_weights Valor inicial de los pesos
        *\param init_bias Valor inicial de las bias
        *
        **/
        Perceptron(size_t ins,size_t outs,size_t height,size_t l,O (*activation)(I),W init_weights,B init_bias) : layers(l),inputs(ins)
        {
            input() = Layer<I,W,O,B>(inputs,activation,init_weights,init_bias);

            for(size_t l = 1; l < layers.size(); l++)
            {
                layers[l] = Layer<I,W,O,B>(layers[l - 1].height,height,activation,init_weights,init_bias);
            }

            output() = Layer<I,W,O,B>(layers[layers.size() - 2].height,outs,activation,init_weights,init_bias);
        }
        /**
        *\brief Contrulle un perceptron multi-capa
        *\param ins Cantidad de entradas
        *\param outs Cantidad salidas
        *\param height Cantidad de neuronas por capa
        *\param l Cantidad de capas
        *\param activation Funcion de activacion
        *\param init_weights Funcion para inicializar los pesos
        *\param init_bias Funcion para inicializar las bias
        *
        **/
        Perceptron(size_t ins,size_t outs,size_t height,size_t l,O (*activation)(I),W (*init_weights)(size_t n,size_t w),B (*init_bias)(size_t n)) : layers(l),inputs(ins)
        {
            layers[0] = Layer<I,W,O,B>(inputs,init_weights,init_bias);

            for(size_t l = 1; l < layers.size() - 1; l++)
            {
                layers[l] = Layer<I,W,O,B>(layers[l - 1].height,height,activation,init_weights,init_bias);
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
            /*std::cout << "\n";
            input().weights.print(std::cout);
            std::cout << "\n";
            input().bias.print(std::cout);
            std::cout << "\n";*/
            input().outputs = input().weights * inm + input().bias;
            /*input().outputs.print(std::cout);
            std::cout << "\n";*/

            for(size_t layer = 1; layer < layers.size(); layer++)
            {
                layers[layer].outputs = layers[layer].weights * layers[layer - 1].outputs + layers[layer].bias;
                //layers[layer].outputs.print(std::cout);
            }
        }
        void spread(core::array<core::array<I>>& ins)
        {
            for(size_t data = 0; data < ins.size(); data++)
            {
                spread(ins[data]);
            }
        }

        /**
        *\brief Optiene la capa de salida
        *
        */
        Layer<I,W,O,B>& output()
        {
            return layers[layers.size() - 1];
        }
        /**
        *\brief Optiene la capa de entrada
        *
        */
        Layer<I,W,O,B>& input()
        {
            return layers[0];
        }

        size_t size() const
        {
            return layers.size();
        }

        Layer<I,W,O,B> const& operator [](size_t i) const
        {
            return layers[i];
        }
        Layer<I,W,O,B>& operator [](size_t i)
        {
            return layers[i];
        }

    private:
        core::array<Layer<I,W,O,B>> layers;
        size_t inputs;

    protected:
    public:

    };
}


#endif // PERCEPTRON_HH_INCLUDED
