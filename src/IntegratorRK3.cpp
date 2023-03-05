/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorRK3.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorRK3::IntegratorRK3(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state((pModel) ? new double [pModel->GetDim()] : NULL)
  ,m_k1((pModel) ? new double [pModel->GetDim()] : NULL)
  ,m_k2((pModel) ? new double [pModel->GetDim()] : NULL)
  ,m_k3((pModel) ? new double [pModel->GetDim()] : NULL)
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "RK3 (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorRK3::~IntegratorRK3()
{
  delete [] m_state;
  delete [] m_k1;
  delete [] m_k2;
  delete [] m_k3;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorRK3::SingleStep()
{
  Evaluate(m_state, m_k1, 0, m_time, m_k1);
  Evaluate(m_state, m_k1, m_h*0.5, m_time, m_k2);
  Evaluate(m_state, m_k3, m_h, m_time, m_k3);

  for (std::size_t i=0; i<m_pModel->GetDim(); ++i)
    m_state[i] += m_h/6 * (m_k1[i] + 4*m_k2[i] + m_k3[i]);

  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorRK3::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
  {
    m_state[i] = state[i];
    m_k1[i] = 0;
    m_k2[i] = 0;
    m_k3[i] = 0;
  }

  m_time = 0;
}

//------------------------------------------------------------------------------
double* IntegratorRK3::GetState() const
{
  return m_state;
}


