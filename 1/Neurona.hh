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
        const I* input;
        W weight;
    };

    template<core::number I,core::number O = I> O funtion(I);

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
        *\brief Enlaza la neurona actual(entradas) con la capa indicada, input i con neurona i.
        *
        **/
        void link(LAYER& layer)
        {
            if(layer.size() != BASE::size()) throw std::out_of_range("La cantidad neuranas en la capa indicada deve coincidir con la cantidad de entrada en la neurona, " + std::to_string(layer.size()) + " y " + std::to_string(BASE::size()));

            for(size_t i = 0; i < BASE::size(); i++)
            {
                BASE::at(i).input = &layer[i].output;
            }
        }

        /**
        *\brief evalua la suma ponderada de la neuronal y genera el resultado de output
        *
        **/
        void sigma()
        {
            output = 0;
            for(size_t i = 0; i < BASE::size(); i++)
            {
                //std::cout << "\tweight: " << BASE::at(i).weight << "\n";
                //std::cout << "\tinput: " << *BASE::at(i).input << "\n";
                output += BASE::at(i).weight * (*BASE::at(i).input);
            }
            //std::cout << "\toutput: " << output << "\n\n";
        }
        void adjust(W w)
        {
            for(size_t i = 0; i < BASE::size(); i++)
            {
                BASE::at(i).weight *= w;
            }
        }


    public:
        O output;
    };

    template<core::number I,core::number W = I,core::number O = I> class Cumulus : public core::array<core::array<Neurona<I,W>>>
    {
    public:
        typedef core::array<core::array<Neurona<I,W>>> BASE;
        typedef core::array<Neurona<I,W>> LAYER;
        typedef Neurona<I,W> NEURONA;

        O dev(I d)
        {
            return (*dereivation)(d);
        }

    private:
        O (*activation)(I);
        O (*dereivation)(I);

    protected:

    public:
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
        LAYER& output()
        {
            return BASE::at(BASE::size() - 1);
        }
        size_t layers()const
        {
            return BASE::size();
        }
        /**
        *\brief
        **/
        void link()
        {
            for(size_t i = 1; i < layers() - 1; i++)//en la capa i
            {
                //std::cout << "\nLinking Capa " << i << "\n";
                for(size_t j = 0; j < layer(i).size(); j++)//para la neurona j
                {
                    layer(i).at(j).link(layer(i - 1));
                }
            }
            //std::cout << "\nLinking Capa " << layers() - 1 << "\n";
            for(size_t j = 0; j < output().size(); j++)//para la neurona j
            {
                output().at(j).link(layer(layers() - 2));
            }
            //std::cout << "\nLinked\n";
        }
        /*W weights(W e,LAYER& from,size_t j)const
        {
            W r = 0;
            for(size_t i = 0; i < from.size(); i++)
            {
                r += from.at(i).at(j).weight * e;
            }

            return r;
        }*/
        W weights(LAYER& from,size_t j)const
        {
            W r = 0;
            for(size_t i = 0; i < from.size(); i++)
            {
                r += from.at(i).at(j).weight;
            }

            return r;
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
        Cumulus(size_t inputs,size_t outputs,size_t layers,size_t amoung,O (*act)(I),O (*dev)(I)) : BASE(layers),activation(act),dereivation(dev)
        {
            if(layers == 0) throw std::domain_error("Imposible un red sin capas");
            if(layers == 1) throw std::domain_error("Imposible un red sin capas ocultas");
            if(layers == 2) throw std::domain_error("Imposible un red sin capas ocultas");
            if(BASE::size() != layers) throw std::domain_error("Imposible un red sin capas");

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
            output().resize(outputs);
            for(size_t i = 0; i < outputs; i++)
            {
                output().at(i).resize(amoung);
            }

            //
            link();
        }

        void spread(const core::array<I>& ds)
        {
            for(size_t i = 0; i < input().size(); i++)
            {
                if(input().at(i).size() != ds.size()) throw std::out_of_range("spreading : La cantida de datos " + std::to_string(ds.size()) + " no  coincide con la cantidad de entradas " + std::to_string(input().at(i).size()) + ", dato " + std::to_string(i)) ;
                for(size_t j = 0; j < input().at(i).size(); j++)
                {
                    input().at(i).at(j).input = &ds[j];
                }
            }

            for(size_t i = 0; i < layers(); i++)
            {
                for(size_t j = 0; j < layer(i).size(); j++)
                {
                    //std::cout << "neurona : " << i << "," << j << "\n";
                    layer(i).at(j).sigma();
                }
            }

        }
    };


}


#endif // OCTETOS_NEURONAL_NEURONA_HH
