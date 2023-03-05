/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorADB6.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstring>


//------------------------------------------------------------------------------
IntegratorADB6::IntegratorADB6(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state()
  ,m_f()
{
  if (pModel == NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  m_c[0] =  4277.0 / 1440.0;
  m_c[1] = -7923.0 / 1440.0;
  m_c[2] =  9982.0 / 1440.0;
  m_c[3] = -7298.0 / 1440.0;
  m_c[4] =  2877.0 / 1440.0;
  m_c[5] =  -475.0 / 1440.0;

  m_state = new double[m_dim];
  for (unsigned i = 0; i < 6; ++i)
  {
    m_f[i] = new double[m_dim];
    std::memset(m_f[i], 0, m_dim*sizeof(double));
  }

  std::stringstream ss;
  ss << "ADB6 (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorADB6::~IntegratorADB6()
{
  delete [] m_state;

  for (unsigned i = 0; i < 6; ++i)
    delete [] m_f[i];
}

//------------------------------------------------------------------------------
void IntegratorADB6::Reverse()
{
  m_h *= -1;
  SetInitialState(GetState());
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorADB6::SingleStep()
{
  for (std::size_t i = 0; i < m_dim; ++i)
  {
    m_state[i] += m_h * (m_c[0] * m_f[5][i] +
                         m_c[1] * m_f[4][i] +
                         m_c[2] * m_f[3][i] +
                         m_c[3] * m_f[2][i] +
                         m_c[4] * m_f[1][i] +
                         m_c[5] * m_f[0][i]);

    m_f[0][i] = m_f[1][i];
    m_f[1][i] = m_f[2][i];
    m_f[2][i] = m_f[3][i];
    m_f[3][i] = m_f[4][i];
    m_f[4][i] = m_f[5][i];
  }

  m_time += m_h;
  m_pModel->Eval(m_state, m_time, m_f[5]);

}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorADB6::SetInitialState(double *state)
{
  for (unsigned i = 0; i < m_dim; ++i)
    m_state[i] = state[i];

  m_time = 0;
  double k1[m_dim],
         k2[m_dim],
         k3[m_dim],
         k4[m_dim],
         tmp[m_dim];

  for (std::size_t n=0; n<5; ++n)
  {
    // k1
    m_pModel->Eval(m_state, m_time, k1);
    for (std::size_t i=0; i<m_dim; ++i)
      tmp[i] = m_state[i] + m_h*0.5 * k1[i];

    // k2
    m_pModel->Eval(tmp, m_time + m_h*0.5, k2);
    for (std::size_t i=0; i<m_dim; ++i)
      tmp[i] = m_state[i] + m_h*0.5 * k2[i];

    // k3
    m_pModel->Eval(tmp, m_time + m_h*0.5, k3);
    for (std::size_t i=0; i<m_dim; ++i)
      tmp[i] = m_state[i] + m_h * k3[i];

    // k4
    m_pModel->Eval(tmp, m_time + m_h, k4);

    for (std::size_t i=0; i<m_dim; ++i)
    {
      m_state[i] += m_h/6 * (k1[i] + 2*(k2[i] + k3[i]) + k4[i]);
      m_f[n][i] = k1[i];
    }

    m_time += m_h;
  }
  m_pModel->Eval(m_state, m_time, m_f[5]);
}

//------------------------------------------------------------------------------
double* IntegratorADB6::GetState() const
{
  return m_state;
}


