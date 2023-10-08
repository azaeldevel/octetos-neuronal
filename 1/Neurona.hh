#ifndef OCTETOS_NEURONAL_NEURONA_HH
#define OCTETOS_NEURONAL_NEURONA_HH

#include <numbers/0/common.hh>
#include <core/3/array.hh>
#include <core/3/numbers.hh>

namespace oct::neu::v0
{
    namespace numbers = oct::nums::v0;
    namespace core = oct::core::v3;

    template<core::number I,core::number W = I> struct Axion
    {
        I* input;
        W weight;
    };

    template<core::number I,core::number W = I,core::number O = I> class Neurona : public core::array<Axion<I,W>>
    {
    public:
        typedef core::array<Axion<I,W>> BASE;
        typedef core::array<Neurona> LAYER;

    private:
    protected:
    public:
        Neurona() = default;
        Neurona(size_t size) : BASE(size)
        {
        }
        /**
        *\brief Enlaza la neurona actual(entradas) con la capa indicada.
        *
        **/
        void link(LAYER& layer)
        {
            //if(layer.size() != BASE::size()) throw std::domain_error("La cantidad de neuronal en la capa no coincide con ls entreda de esta neuroana");

            for(size_t i = 0; i < BASE::size(); i++)
            {
                //BASE::at(i).input = &layer[i].output;
            }
        }

    public:
        O output;
    };

    struct Dimension
    {
        size_t layers;
        size_t amoung;//por capa interior
        size_t inputs;//de la primera capa, por default de cada neurona
        size_t outputs;//neuronas de slida
    };

    template<core::number I,core::number W = I> class Cumulus : public core::array<core::array<Neurona<I,W>>>
    {
    public:
        typedef core::array<core::array<Neurona<I,W>>> BASE;
        typedef core::array<Neurona<I,W>> LAYER;
        typedef Neurona<I,W> NEURONA;

    private:
        //size_t layers,amoung,inputs,outputs;
        //Dimension dimension;
    protected:

        LAYER& layer(size_t index)
        {
            return BASE::at(index);
        }
        NEURONA& neurona(size_t l,size_t n)
        {
            return BASE::at(l).at(n);
        }
        LAYER& input()
        {
            return BASE::at(0);
        }
        size_t layers()const
        {
            return BASE::size();
        }
        void link()
        {
            for(size_t i = 1; i < layers() - 1; i++)//en la capa i
            {
                std::cout << "\nCapa " << i << "\n";
                for(size_t j = 0; j < layer(i).size(); j++)//para la neurona j
                {
                    BASE::at(i).at(j).link(layer(i - 1));
                }
            }
        }

    public:
        Cumulus() = default;
        /**
        *\brief
        *\param inputs de la primera capa, por default de cada neurona
        *\param outputs neuronas de salida
        *\param layers cantidad de capas
        *\param amoung cantidad de neuronas en capa interior
        */
        Cumulus(size_t inputs,size_t outputs,size_t layers,size_t amoung) : BASE(layers)
        {
            if(layers == 0) throw std::domain_error("Imposible un red sin capas");
            if(layers == 1) throw std::domain_error("Imposible un red sin capas ocultas");
            if(layers == 2) throw std::domain_error("Imposible un red sin capas ocultas");

            //primer capa
            layer(0).resize(inputs);
            for(size_t i = 0; i < inputs; i++)
            {
                layer(0).at(i).resize(inputs);
            }
            //primer capa oculta
            layer(1).resize(amoung);
            for(size_t i = 0; i < amoung; i++)
            {
                layer(1).at(i).resize(inputs);
            }
            //capas internas
            for(size_t i = 2; i < layers - 1; i++)
            {
                layer(i).resize(amoung);
                for(size_t j = 0; j < amoung; j++)
                {
                    layer(i).at(j).resize(amoung);
                }
            }

            //capa de salida
            layer(layers - 1).resize(outputs);
            for(size_t i = 0; i < outputs; i++)
            {
                layer(layers - 1).at(i).resize(amoung);
            }

            link();
        }
    };


}


#endif // OCTETOS_NEURONAL_NEURONA_HH
