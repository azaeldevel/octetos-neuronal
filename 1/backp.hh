#ifndef OCTETOS_NEURONAL_BACKP_HH
#define OCTETOS_NEURONAL_BACKP_HH

#include "Neurona.hh"

namespace oct::neu::v0
{

    template<core::number I,core::number W = I> class Backp
    {
    public:
    private:
    protected:
    public:
        void training(Cumulus<I,W>& cumulus, const core::array<core::array<I>>& bach)
        {
        }
    };

}

#endif // OCTETOS_NEURONAL_BACKP_HH
