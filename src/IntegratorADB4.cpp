#include "IntegratorADB4.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


IntegratorADB4::IntegratorADB4(IModel *pModel, double h)
    :IIntegrator(pModel, h)
    ,_state()
    ,_f()
{
    if (pModel == nullptr)
        throw std::runtime_error("Model pointer may not be NULL.");

    _state = new double[m_dim];
    for (unsigned i = 0; i < 4; ++i)
    {
        _f[i] = new double[m_dim];
    }

    std::stringstream ss;
    ss << "ADB4 (dt=" << m_h << ")";
    SetID(ss.str());
}

IntegratorADB4::~IntegratorADB4()
{
    delete[] _state;

    for (unsigned i = 0; i < 4; ++i)
    {
        delete _f[i];
    }
}


/** \brief Performs a single integration step. */
void IntegratorADB4::SingleStep()
{
    for (std::size_t i = 0; i < m_dim; ++i)
    {
        _state[i] += m_h / 24.0 * (55 * _f[3][i] - 59 * _f[2][i] + 37 * _f[1][i] - 9 * _f[0][i]);

        _f[0][i] = _f[1][i];
        _f[1][i] = _f[2][i];
        _f[2][i] = _f[3][i];
    }

    m_time += m_h;
    m_pModel->Eval(_state, m_time, _f[3]);
}

/** \brief Sets the initial state of the simulation. */
void IntegratorADB4::SetInitialState(double *state)
{
    /*
      // x0
      for (unsigned i = 0; i < m_dim; ++i)
        m_state[i] = state[i];

      m_time = 0;
      Evaluate(m_state, m_f[0], 0, m_time, m_f[0]);

      // x1
      for (unsigned i = 0; i < m_dim; ++i)
        m_state[i] += m_h * m_f[0][i];

      m_time += m_h;
      Evaluate(m_state, m_f[1], 0, m_time, m_f[1]);

      // x2
      for (unsigned i = 0; i < m_dim; ++i)
        m_state[i] += m_h * m_f[1][i];

      m_time += m_h;
      Evaluate(m_state, m_f[2], 0, m_time, m_f[2]);

      // x3
      for (unsigned i = 0; i < m_dim; ++i)
        m_state[i] += m_h * m_f[2][i];

      m_time += m_h;
    */
    for (unsigned i = 0; i < m_dim; ++i)
        _state[i] = state[i];

    m_time = 0;
    double k1[m_dim],
        k2[m_dim],
        k3[m_dim],
        k4[m_dim],
        tmp[m_dim];

    for (std::size_t n = 0; n < 3; ++n)
    {
        // k1
        m_pModel->Eval(_state, m_time, k1);
        for (std::size_t i = 0; i < m_dim; ++i)
            tmp[i] = _state[i] + m_h * 0.5 * k1[i];

        // k2
        m_pModel->Eval(tmp, m_time + m_h * 0.5, k2);
        for (std::size_t i = 0; i < m_dim; ++i)
            tmp[i] = _state[i] + m_h * 0.5 * k2[i];

        // k3
        m_pModel->Eval(tmp, m_time + m_h * 0.5, k3);
        for (std::size_t i = 0; i < m_dim; ++i)
            tmp[i] = _state[i] + m_h * k3[i];

        // k4
        m_pModel->Eval(tmp, m_time + m_h, k4);

        for (std::size_t i = 0; i < m_dim; ++i)
        {
            _state[i] += m_h / 6 * (k1[i] + 2 * (k2[i] + k3[i]) + k4[i]);
            _f[n][i] = k1[i];
        }

        m_time += m_h;
    }

    m_pModel->Eval(_state, m_time, _f[3]);
}


double *IntegratorADB4::GetState() const
{
    return _state;
}
