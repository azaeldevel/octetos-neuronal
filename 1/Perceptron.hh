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

    float sigmoid_D(float x)
    {
        return x * (1 - x);
    }
    float sigmoid(float x)
    {
        return 1/(1 + std::pow(std::numbers::e,-x));
    }

    float identity(float d)
    {
        return d;
    }
    float identity_D(float d)
    {
        return 1;
    }



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
        //numbers::matrix<B> bias;
        numbers::matrix<O> outputs;
        size_t height;
        O (*activation)(I);

        Layer() = default;
        Layer(size_t inputs,O (*a)(I)) : height(1),activation(a)//perceptron simple
        {
            weights.resize(height,inputs);
            //bias.resize(height,1);
            outputs.resize(height,1);
            //bias[0][0] = 0;
            outputs[0][0] = 0;
        }
        Layer(size_t inputs,size_t h,O (*a)(I)) : height(h),activation(a)//perceptron simple
        {
            weights.resize(height,inputs);
            //bias.resize(height,1);
            outputs.resize(height,1);
            for(size_t n = 0; n < height; n++)
            {
                outputs[n][0] = 0;
            }
        }

        Layer(Model<I,W,O,B> const& m)//perceptron multi-capa
        {
        }

        Layer(const Layer& o) : weights(o.weights),outputs(o.outputs),height(o.height),activation(o.activation)
        {
        }

        Layer& operator = (const Layer& o)
        {
            weights = o.weights;
            outputs = o.outputs;
            height = o.height;
            activation = o.activation;

            return *this;
        }
    };

    /**
    *\brief Crea un Perceptron simple un multi-capa
    **/
    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Perceptron : public core::array<Layer<I,W,O,B>>
    {
    public:
        typedef core::array<Layer<I,W,O,B>> BASE;
    public://contructores

        /**
        *\brief Contrulle un perceptron simple
        *\param ins Cantidad de entradas
        *\param activation Funcion de activacion
        **/
        Perceptron(size_t ins,O (*activation)(I)) : BASE(1),inputs(ins)
        {
            BASE::front() = Layer<I,W,O,B>(1,inputs,activation);
        }

        /**
        *\brief Contrulle un perceptron multi-capa
        *\param ins Cantidad de entradas
        *\param outs Cantidad salidas
        *\param height Cantidad de neuronas por capa
        *\param l Cantidad de capas
        *\param activation Funcion de activacion
        *\param init_weights Valor inicial de los pesos
        *
        **/
        Perceptron(size_t ins,size_t outs,size_t height,size_t l,O (*activation)(I)) : BASE(l),inputs(ins)
        {
            BASE::front() = Layer<I,W,O,B>(1,inputs,activation);

            for(size_t l = 1; l < BASE::size(); l++)
            {
                BASE::at(l) = Layer<I,W,O,B>(BASE::at(l - 1).height,height,activation);
            }

            BASE::back() = Layer<I,W,O,B>(BASE::at(BASE::size() - 2).height,outs,activation);
        }

        Perceptron(core::array<Model<I,W,O>> model);

    public:
        void feedforward(const core::array<I>& ins)
        {
            //std::cout << "\n";
            //inm.print(std::cout);
            /*std::cout << "\n";
            input().weights.print(std::cout);
            std::cout << "\n";
            input().bias.print(std::cout);
            std::cout << "\n";*/
            for(size_t i = 0; i < BASE::front().weights.rows(); i++)
            {
                BASE::front().outputs[i][0] = ins[i] * BASE::front().weights[i][0];//+ BASE::front().bias[i][0]
            }
            /*BASE::front().outputs.print(std::cout);
            std::cout << "\n";*/

            for(size_t layer = 1; layer < BASE::size(); layer++)
            {
                BASE::at(layer).outputs = BASE::at(layer).weights * BASE::at(layer - 1).outputs;// + BASE::at(layer).bias
                //layers[layer].outputs.print(std::cout);
            }
        }
        void feedforward(const core::array<core::array<I>>& ins)
        {
            for(size_t data = 0; data < ins.size(); data++)
            {
                feedforward(ins[data]);
            }
        }



    private:
        //core::array<Layer<I,W,O,B>> layers;
        size_t inputs;

    protected:
    public:

    };
}


#endif // PERCEPTRON_HH_INCLUDED
