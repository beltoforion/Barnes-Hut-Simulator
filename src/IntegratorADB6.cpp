#include "IntegratorADB6.h"

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstring>


IntegratorADB6::IntegratorADB6(IModel *model, double h)
    :IIntegrator(model, h)
    ,_state()
    ,_f()
{
    if (model == nullptr)
        throw std::runtime_error("Model pointer may not be NULL.");

    _c[0] = 4277.0 / 1440.0;
    _c[1] = -7923.0 / 1440.0;
    _c[2] = 9982.0 / 1440.0;
    _c[3] = -7298.0 / 1440.0;
    _c[4] = 2877.0 / 1440.0;
    _c[5] = -475.0 / 1440.0;

    _state = new double[model->GetDim()];
    for (unsigned i = 0; i < 6; ++i)
    {
        _f[i] = new double[model->GetDim()];
        std::memset(_f[i], 0, model->GetDim() * sizeof(double));
    }

    std::stringstream ss;
    ss << "ADB6 (dt=" << m_h << ")";
    SetID(ss.str());
}


IntegratorADB6::~IntegratorADB6()
{
    delete[] _state;

    for (unsigned i = 0; i < 6; ++i)
        delete[] _f[i];
}


void IntegratorADB6::Reverse()
{
    m_h *= -1;
    SetInitialState(GetState());
}


/** \brief Performs a single integration step. */
void IntegratorADB6::SingleStep()
{
    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
    {
        _state[i] += m_h * (_c[0] * _f[5][i] +
                            _c[1] * _f[4][i] +
                            _c[2] * _f[3][i] +
                            _c[3] * _f[2][i] +
                            _c[4] * _f[1][i] +
                            _c[5] * _f[0][i]);

        _f[0][i] = _f[1][i];
        _f[1][i] = _f[2][i];
        _f[2][i] = _f[3][i];
        _f[3][i] = _f[4][i];
        _f[4][i] = _f[5][i];
    }

    m_time += m_h;
    m_pModel->Eval(_state, m_time, _f[5]);
}


/** \brief Sets the initial state of the simulation. */
void IntegratorADB6::SetInitialState(double *state)
{
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _state[i] = state[i];

    m_time = 0;
    double k1[m_pModel->GetDim()],
           k2[m_pModel->GetDim()],
           k3[m_pModel->GetDim()],
           k4[m_pModel->GetDim()],
           tmp[m_pModel->GetDim()];

    for (std::size_t n = 0; n < 5; ++n)
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
    m_pModel->Eval(_state, m_time, _f[5]);
}


double *IntegratorADB6::GetState() const
{
    return _state;
}
