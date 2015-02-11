/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorADB3.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorADB3::IntegratorADB3(IModel *pModel, double h)
: IIntegrator(pModel, h)
, m_state()
, m_f()
{
  if (pModel == NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  m_state = new double[m_dim];
  for (unsigned i = 0; i < 3; ++i)
  {
    m_f[i] = new double[m_dim];
  }

  std::stringstream ss;
  ss << "ADB3 (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorADB3::~IntegratorADB3()
{
  delete [] m_state;

  for (unsigned i = 0; i < 3; ++i)
    delete [] m_f;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorADB3::SingleStep()
{
  for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
  {
    m_state[i] += m_h / 12.0 * (23 * m_f[2][i] -
                                16 * m_f[1][i] +
                                 5 * m_f[0][i]);

    m_f[0][i] = m_f[1][i];
    m_f[1][i] = m_f[2][i];
  }

  m_time += m_h;
  m_pModel->Eval(m_state, m_time, m_f[2]);
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorADB3::SetInitialState(double *state)
{
  for (unsigned i = 0; i < m_dim; ++i)
    m_state[i] = state[i];

  m_time = 0;
  double k1[m_dim],
         k2[m_dim],
         k3[m_dim],
         k4[m_dim],
         tmp[m_dim];

  for (std::size_t n=0; n<2; ++n)
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
  m_pModel->Eval(m_state, m_time, m_f[2]);
}

//------------------------------------------------------------------------------
double* IntegratorADB3::GetState() const
{
  return m_state;
}


