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
        size_t length;//cantidad de neuronas en el layer
        O (*activation)(I);
        O (*dereivation)(I);
    };

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> struct Layer
    {
        numbers::matrix<W> weights;
        numbers::matrix<B> bias;
        numbers::matrix<O> outputs;

        Layer() = default;
        Layer(size_t inputs)//perceptron simple
        {
            weights.resize(1,inputs);
            bias.resize(1,1);
            outputs.resize(1,1);
        }

        Layer(Model<I,W,O,B> const& m)//perceptron multi-capa
        {
        }
    };

    template<core::number I,core::number W = I,core::number O = I,core::number B = I> class Perceptron
    {
    public:
        Perceptron(size_t inputs,O (*activation)(I),O (*dereivation)(I)) : layers(1)
        {
            layers[0] = Layer<I,W,O,B>(inputs);
        }
        Perceptron(core::array<Model<I,W,O>> model);

    public:
        void spread(core::array<core::array<I>> in,core::array<core::array<O>> out)
        {

        }
    private:
        core::array<Layer<I,W,O,B>> layers;

    protected:
    public:

    };
}


#endif // PERCEPTRON_HH_INCLUDED
