#ifndef OCTETOS_NEURONAL_BACKP_HH
#define OCTETOS_NEURONAL_BACKP_HH

#include "Neurona.hh"
#include <numbers/0/arithmetic.hh>

namespace oct::neu::v0
{

    template<core::number I,core::number W = I,core::number O = I> class Backp
    {
    public:
    private:
    protected:
    public:
        void training(Cumulus<I,W>& cumulus, const core::array<core::array<I>>& bachI, const core::array<core::array<I>>& bachO)
        {
            if(bachI.size() != bachO.size()) throw std::out_of_range("La cantidad de datos no coincide");

            O sigma;

            for(size_t d = 0; d < bachI.size(); d++)
            {
                sigma = 0;
                for(size_t i = 0; i < cumulus.output().size(); i++)
                {
                    if(cumulus.output().size() != bachO[d].size()) throw std::out_of_range("La cantidad de salidas de la red , " + std::to_string(cumulus.output().size()) + ",  no coincide con la cantiad de datos de salida "  + std::to_string(bachO[d].size()));
                    //bachO[d][i];
                    //sigma += numbers::sqrterr(bachO[d][i],cumulus.output().at(i).output);
                }

                /*for(int i = cumulus.layers() - 1; i >= 0 ; i--)
                {

                    for(size_t j = 0; j < cumulus.layer(i).size(); j++)
                    {
                        std::cout << "neurona : " << i << "," << j << "\n";
                    }
                }*/

            }
        }
    };

}

#endif // OCTETOS_NEURONAL_BACKP_HH
