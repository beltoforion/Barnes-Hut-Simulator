/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorRK4.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorRK4::IntegratorRK4(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state(new double [m_dim])
  ,m_tmp(new double [m_dim])
  ,m_k1(new double [m_dim])
  ,m_k2(new double [m_dim])
  ,m_k3(new double [m_dim])
  ,m_k4(new double [m_dim])
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "RK4 (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorRK4::~IntegratorRK4()
{
  delete [] m_state;
  delete [] m_tmp;
  delete [] m_k1;
  delete [] m_k2;
  delete [] m_k3;
  delete [] m_k4;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorRK4::SingleStep()
{
  assert(m_pModel);

  // k1
  m_pModel->Eval(m_state, m_time, m_k1);
  for (std::size_t i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h*0.5 * m_k1[i];

  // k2
  m_pModel->Eval(m_tmp, m_time + m_h*0.5, m_k2);
  for (std::size_t i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h*0.5 * m_k2[i];

  // k3
  m_pModel->Eval(m_tmp, m_time + m_h*0.5, m_k3);
  for (std::size_t i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h * m_k3[i];

  // k4
  m_pModel->Eval(m_tmp, m_time + m_h, m_k4);

  for (std::size_t i=0; i<m_dim; ++i)
    m_state[i] += m_h/6 * (m_k1[i] + 2*(m_k2[i]+m_k3[i]) + m_k4[i]);

  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorRK4::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
  {
    m_state[i] = state[i];
    m_k1[i] = 0;
    m_k2[i] = 0;
    m_k3[i] = 0;
    m_k4[i] = 0;
  }

  m_time = 0;
}

//------------------------------------------------------------------------------
double* IntegratorRK4::GetState() const
{
  return m_state;
}


