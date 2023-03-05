/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorHeun.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorHeun::IntegratorHeun(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state(new double[m_dim])
  ,m_tmp(new double[m_dim])
  ,m_k1(new double[m_dim])
  ,m_k2(new double[m_dim])
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "Heun (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorHeun::~IntegratorHeun()
{
  delete [] m_state;
  delete [] m_tmp;
  delete [] m_k1;
  delete [] m_k2;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorHeun::SingleStep()
{
  m_pModel->Eval(m_state, m_time, m_k1);

  for (std::size_t i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + 2.0/3.0 * m_h * m_k1[i];

  m_pModel->Eval(m_tmp, m_time + 2.0/3.0 * m_h, m_k2);

  for (std::size_t i=0; i<m_dim; ++i)
    m_state[i] += m_h/4.0 * (m_k1[i] + 3*m_k2[i]);

  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorHeun::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
  {
    m_state[i] = state[i];
    m_k1[i] = 0;
    m_k2[i] = 0;
  }

  m_time = 0;
}

//------------------------------------------------------------------------------
double* IntegratorHeun::GetState() const
{
  return m_state;
}


