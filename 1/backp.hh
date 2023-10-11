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
        O Sk(size_t n,size_t d) const
        {
            return cumulus.output().at(n).output - bach_out[d][n];
        }
        O Sj(size_t n,O sk) const
        {
            auto K = cumulus.output();
            size_t j = cumulus.layers() - 1;
            O sj = 0;
            for(size_t w = 0; w < K.at(n).size(); w++)
            {
                sj += K.at(n).at(w).weight * sk;
            }

            return cumulus.dev(K.at(n).output) * sj;
        }
        O Sj(size_t j,size_t n,O sk) const
        {
            auto K = cumulus.layer(j + 1);
            O sj = 0;
            for(size_t w = 0; w < K.at(n).size(); w++)
            {
                sj += K.at(n).at(w).weight * sk;
            }

            return cumulus.dev(K.at(n).output) * sj;
        }

        O training()
        {
            if(bach_in.size() != bach_out.size()) throw std::out_of_range("La cantidad de datos no coincide");

            O m = 0,E = 0,e;
            //core::array<O> E(cumulus.output().size());
            for(size_t d = 0; d < bach_in.size(); d++)
            {
                cumulus.spread(bach_in[d]);

                //std::cout << "dato : " << d << "\n";

                //capa de salida
                size_t j = cumulus.layers() - 1;
                for(size_t n = 0; n < cumulus.output().size(); n++)
                {
                    e = Sj(n,Sk(n,d)) * cumulus.neurona(j,n).output;
                    cumulus.neurona(j,n).adjust(e * ratio);
                    E += e;
                }
                E /= O(cumulus.output().size());
                m += E;
                //capa penultima
                j = cumulus.layers() - 2;
                for(size_t n = 0; n < cumulus.layer(cumulus.layers() - 2).size(); n++)
                {
                    //Sj(j,n,Sk(n,d));
                    //e = Sj(j,n,Sk(n,d)) * cumulus.neurona(j,n).output;
                    //cumulus.neurona(j,n).adjust(e * ratio);
                }
                //
                /*for(int j = cumulus.layers() - 3; j >= 0 ; j--)
                {
                    for(size_t n = 0; n < cumulus.layer(j).size(); n++)
                    {
                        e = Sj(j,n,Sk(n,d)) * cumulus.neurona(j,n).output;
                        cumulus.neurona(j,n).adjust(e * ratio);
                    }
                }*/

            }
            m /= O(bach_in.size());

            return m;
        }
    };

}

#endif // OCTETOS_NEURONAL_BACKP_HH
