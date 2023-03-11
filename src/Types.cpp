#include "Types.h"

//--- Standard includes ------------------------------------------------------------------
#include <cassert>
#include <cstdlib>


ParticleData::ParticleData()
    : _pState(nullptr), _pAuxState(nullptr)
{}


ParticleData::ParticleData(PODState *pState, PODAuxState *pAuxState)
    : _pState(pState), _pAuxState(pAuxState)
{
    assert(_pState);
    assert(_pAuxState);
}


ParticleData::ParticleData(const ParticleData &ref)
    : _pState(ref._pState), _pAuxState(ref._pAuxState)
{}


ParticleData &ParticleData::operator=(const ParticleData &ref)
{
    if (this != &ref)
    {
        _pState = ref._pState;
        _pAuxState = ref._pAuxState;
    }

    return *this;
}


void ParticleData::Reset()
{
    _pState = nullptr;
    _pAuxState = nullptr;
}


bool ParticleData::IsNull() const
{
    return _pState && _pAuxState;
}
