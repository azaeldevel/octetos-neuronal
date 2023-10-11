#ifndef OCTETOS_NEURONAL_BACKP_HH
#define OCTETOS_NEURONAL_BACKP_HH

#include "Neurona.hh"
#include <numbers/0/arithmetic.hh>

namespace oct::neu::v0
{

    template<core::number I,core::number W = I,core::number O = I> class Backp
    {
    public:
        Backp(Cumulus<I,W>& c, const core::array<core::array<I>>& bachI, const core::array<core::array<O>>& bachO,float r) : bach_in(bachI),bach_out(bachO),cumulus(c),ratio(r)
        {
        }

    private:
        const core::array<core::array<I>>& bach_in;
        const core::array<core::array<O>>& bach_out;
        Cumulus<I,W>& cumulus;
        W ratio;

    protected:
    public:

        O error(size_t i) const
        {
            if(i == 0 ) return 0;


        }

        O training()
        {
            if(bach_in.size() != bach_out.size()) throw std::out_of_range("La cantidad de datos no coincide");

            O E = 0,e,ek;
            //core::array<O> E(cumulus.output().size());
            for(size_t d = 0; d < bach_in.size(); d++)
            {
                cumulus.spread(bach_in[d]);

                std::cout << "dato : " << d << "\n";
                e = 0;
                for(size_t i = 0; i < cumulus.output().size(); i++)
                {
                    if(cumulus.output().size() != bach_out[d].size()) throw std::out_of_range("La cantidad de salidas de la red , " + std::to_string(cumulus.output().size()) + ",  no coincide con la cantiad de datos de salida "  + std::to_string(bach_out[d].size()));
                    //std::cout << "\t" << bachO[d][i] << " - " << cumulus.output().at(i).output << "\n";
                    //E[i] = numbers::sqrterr(bachO[d][i],cumulus.output().at(i).output);
                    e += numbers::sqrterr(bach_out[d][i],cumulus.output().at(i).output);
                }
                e /= O(cumulus.output().size());
                std::cout << "\tError = " << e << "\n";
                /*std::cout << "\tError = ";
                E.print(std::cout);
                std::cout << "\n";*/
                E += e;

                for(size_t i = 0; i < cumulus.output().size(); i++)
                {
                    for(size_t j = 0; j < cumulus.output().at(i).size(); j++)
                    {
                        ek = cumulus.weights(e * ratio, cumulus.output(),j);
                        //std::cout << "\t\tek : " << ek << "\n";
                        cumulus.neurona(i,j).adjust(ek);
                    }
                }
                for(int i = cumulus.layers() - 2; i >= 0 ; i--)
                {
                    for(size_t j = 0; j < cumulus.layer(i).size(); j++)
                    {
                        //std::cout << "\t\tneurona : " << i << "," << j << "\n";
                        //std::cout << "\te : " << e << "\n";
                        ek = cumulus.weights(e * ratio, cumulus.layer(i + 1),j);
                        //std::cout << "\t\tek : " << ek << "\n";
                        cumulus.neurona(i,j).adjust(ek);
                    }
                }

            }
            E /= O(bach_in.size());

            return E;
        }
    };

}

#endif // OCTETOS_NEURONAL_BACKP_HH
