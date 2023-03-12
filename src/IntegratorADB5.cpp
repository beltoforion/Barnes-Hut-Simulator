#include "IntegratorADB5.h"

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstring>


IntegratorADB5::IntegratorADB5(IModel *pModel, double h)
    :IIntegrator(pModel, h)
    ,_state()
    ,_f()
    ,_rk4(pModel, h)
{
    if (pModel == nullptr)
        throw std::runtime_error("Model pointer may not be NULL.");

    _c[0] = 1901.0 / 720.0;
    _c[1] = 1387.0 / 360.0;
    _c[2] = 109.0 / 30.0;
    _c[3] = 637.0 / 360.0;
    _c[4] = 251.0 / 720.0;

    _state = new double[pModel->GetDim()];
    for (unsigned i = 0; i < 6; ++i)
    {
        _f[i] = new double[pModel->GetDim()];
        std::memset(_f[i], 0, pModel->GetDim() * sizeof(double));
    }

    std::stringstream ss;
    ss << "ADB5 (dt=" << m_h << ")";
    SetID(ss.str());
}


IntegratorADB5::~IntegratorADB5()
{
    delete[] _state;

    for (unsigned i = 0; i < 6; ++i)
        delete[] _f[i];
}


/** \brief Performs a single integration step. */
void IntegratorADB5::SingleStep()
{
    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
    {
        _state[i] += m_h * (_c[0] * _f[4][i] -
                            _c[1] * _f[3][i] +
                            _c[2] * _f[2][i] -
                            _c[3] * _f[1][i] +
                            _c[4] * _f[0][i]);

        _f[0][i] = _f[1][i];
        _f[1][i] = _f[2][i];
        _f[2][i] = _f[3][i];
        _f[3][i] = _f[4][i];
    }

    m_time += m_h;
    m_pModel->Eval(_state, m_time, _f[4]);
}


/** \brief Sets the initial state of the simulation. */
void IntegratorADB5::SetInitialState(double *state)
{
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _state[i] = state[i];

    m_time = 0;
    double k1[m_pModel->GetDim()],
           k2[m_pModel->GetDim()],
           k3[m_pModel->GetDim()],
           k4[m_pModel->GetDim()],
           tmp[m_pModel->GetDim()];

    for (std::size_t n = 0; n < 4; ++n)
    {
        // k1
        m_pModel->Eval(_state, m_time, k1);
        for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
            tmp[i] = _state[i] + m_h * 0.5 * k1[i];

        // k2
        m_pModel->Eval(tmp, m_time + m_h * 0.5, k2);
        for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
            tmp[i] = _state[i] + m_h * 0.5 * k2[i];

        // k3
        m_pModel->Eval(tmp, m_time + m_h * 0.5, k3);
        for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
            tmp[i] = _state[i] + m_h * k3[i];

        // k4
        m_pModel->Eval(tmp, m_time + m_h, k4);

        for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
        {
            _state[i] += m_h / 6 * (k1[i] + 2 * (k2[i] + k3[i]) + k4[i]);
            _f[n][i] = k1[i];
        }

        m_time += m_h;
    }

    m_pModel->Eval(_state, m_time, _f[4]);
}


double *IntegratorADB5::GetState() const
{
    return _state;
}
