#include "IntegratorRKF4.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <iostream>


IntegratorRKF4::IntegratorRKF4(IModel *pModel, double h)
    :IIntegrator(pModel, h)
    ,_state(new double[pModel->GetDim()])
    ,_tmp(new double[pModel->GetDim()])
    ,_k1(new double[pModel->GetDim()])
    ,_k2(new double[pModel->GetDim()])
    ,_k3(new double[pModel->GetDim()])
    ,_k4(new double[pModel->GetDim()])
    ,_k5(new double[pModel->GetDim()])
    ,_k6(new double[pModel->GetDim()])
    ,_error(new double[pModel->GetDim()])
    ,_maxErr(0.005)
{
    if (pModel == nullptr)
        throw std::runtime_error("Model pointer may not be NULL.");

    std::stringstream ss;
    ss << "RKF4 (dt=" << m_h << ")";
    SetID(ss.str());
}


IntegratorRKF4::~IntegratorRKF4()
{
    delete[] _state;
    delete[] _tmp;
    delete[] _k1;
    delete[] _k2;
    delete[] _k3;
    delete[] _k4;
    delete[] _k5;
    delete[] _k6;
    delete[] _error;
}


/** \brief Performs a single integration step. */
void IntegratorRKF4::SingleStep()
{
    assert(m_pModel);

    // k1
    m_pModel->Eval(_state, m_time, _k1);

    // k2
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + 0.25 * m_h * _k1[i];

    m_pModel->Eval(_tmp, m_time + 0.25 * m_h, _k2);

    // k3
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * (3.0 / 32.0 * _k1[i] + 9.0 / 32.0 * _k2[i]);

    m_pModel->Eval(_tmp, m_time + 3.0 / 8.0 * m_h, _k3);

    // k4
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * (1932.0 / 2197.0 * _k1[i] - 7200.0 / 2197.0 * _k2[i] + 7296.0 / 2197.0 * _k3[i]);

    m_pModel->Eval(_tmp, m_time + 12.0 / 13.0 * m_h, _k4);

    // k5
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * (439.0 / 216.0 * _k1[i] - 8.0 * _k2[i] + 3680.0 / 513.0 * _k3[i] - 845.0 / 4104.0 * _k4[i]);

    m_pModel->Eval(_tmp, m_time + m_h, _k5);

    // K6
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _tmp[i] = _state[i] + m_h * (-8.0 / 27.0 * _k1[i] - 2.0 * _k2[i] - 3544.0 / 2565.0 * _k3[i] + 1859.0 / 4104.0 * _k4[i] - 11.0 / 55.0 * _k5[i]);

    m_pModel->Eval(_tmp, m_time + 0.5 * m_h, _k6);

    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
        _state[i] += m_h * (25.0 / 216.0 * _k1[i] +
                             1408.0 / 2565.0 * _k3[i] +
                             2197.0 / 4104.0 * _k4[i] -
                             1.0 / 5.0 * _k5[i]);

    // Fehler
    m_err = 0;
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
    {
        double e = m_h * (1.0 / 360.0 * _k1[i] -
                          128.0 / 4275.0 * _k3[i] -
                          2197.0 / 75240.0 * _k4[i] +
                          1.0 / 50.0 * _k5[i] -
                          2.0 / 55.0 * _k6[i]);
        m_err += e * e;
    }

    m_err = std::sqrt(m_err);

    double h = m_h * std::pow(_maxErr / m_err, 0.25);
    if (h > 2.0 * m_h)
    {
        m_h = h;
    }
    else if (h < m_h)
    {
        m_h = 0.75 * h;
    }

    m_time += m_h;
}


/** \brief Sets the initial state of the simulation. */
void IntegratorRKF4::SetInitialState(double *state)
{
    for (unsigned i = 0; i < m_pModel->GetDim(); ++i)
    {
        _state[i] = state[i];
        _k1[i] = 0;
        _k2[i] = 0;
        _k3[i] = 0;
        _k4[i] = 0;
        _k5[i] = 0;
        _k6[i] = 0;
    }

    m_time = 0;
}


double *IntegratorRKF4::GetState() const
{
    return _state;
}


void IntegratorRKF4::SetMaximumError(double maxErr)
{
    _maxErr = maxErr;
}
