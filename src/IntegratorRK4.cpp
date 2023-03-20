#include "IntegratorRK4.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


IntegratorRK4::IntegratorRK4(IModel *pModel, double h)
    :IIntegrator(pModel, h)
    ,_state(new double[pModel->GetDim()])
    ,_tmp(new double[pModel->GetDim()])
    ,_k1(new double[pModel->GetDim()])
    ,_k2(new double[pModel->GetDim()])
    ,_k3(new double[pModel->GetDim()])
    ,_k4(new double[pModel->GetDim()])
{
    if (pModel == nullptr)
        throw std::runtime_error("Model pointer may not be NULL.");

    std::stringstream ss;
    ss << "RK4 (dt=" << m_h << ")";
    SetID(ss.str());
}


IntegratorRK4::~IntegratorRK4()
{
    delete[] _state;
    delete[] _tmp;
    delete[] _k1;
    delete[] _k2;
    delete[] _k3;
    delete[] _k4;
}


/** \brief Performs a single integration step. */
void IntegratorRK4::SingleStep()
{
    assert(m_pModel);

    // k1
    m_pModel->Eval(_state, m_time, _k1);
    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * 0.5 * _k1[i];

    // k2
    m_pModel->Eval(_tmp, m_time + m_h * 0.5, _k2);
    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * 0.5 * _k2[i];

    // k3
    m_pModel->Eval(_tmp, m_time + m_h * 0.5, _k3);
    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * _k3[i];

    // k4
    m_pModel->Eval(_tmp, m_time + m_h, _k4);

    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
        _state[i] += m_h / 6 * (_k1[i] + 2 * (_k2[i] + _k3[i]) + _k4[i]);

    m_time += m_h;
}


/** \brief Sets the initial state of the simulation. */
void IntegratorRK4::SetInitialState(double *state)
{
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
    {
        _state[i] = state[i];
        _k1[i] = 0;
        _k2[i] = 0;
        _k3[i] = 0;
        _k4[i] = 0;
    }

    m_time = 0;
}


double *IntegratorRK4::GetState() const
{
    return _state;
}
