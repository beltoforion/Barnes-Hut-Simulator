/* 
 * File:   integratoreulerimproved.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:24
 */

#include "IntegratorEulerImproved.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorEulerImproved::IntegratorEulerImproved(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state(new double[m_dim])
  ,m_tmp(new double[m_dim])
  ,m_k1(new double[m_dim])
  ,m_k2(new double[m_dim])
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "Euler mod. (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorEulerImproved::~IntegratorEulerImproved()
{
  delete [] m_state;
  delete [] m_tmp;
  delete [] m_k1;
  delete [] m_k2;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorEulerImproved::SingleStep()
{
  assert(m_pModel);

  m_pModel->Eval(m_state, m_time, m_k1);

  for (std::size_t i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h * m_k1[i];

  m_pModel->Eval(m_tmp, m_time + m_h, m_k2);


  for (std::size_t i=0; i<m_pModel->GetDim(); ++i)
    m_state[i] += m_h/2 * (m_k1[i] + m_k2[i]);

  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorEulerImproved::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
    m_state[i] = state[i];

  m_time = 0;
}

//------------------------------------------------------------------------------
double* IntegratorEulerImproved::GetState() const
{
  return m_state;
}
