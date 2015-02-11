/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorRK5.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorRK5::IntegratorRK5(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state(new double [m_dim])
  ,m_tmp(new double [m_dim])
  ,m_k1(new double [m_dim])
  ,m_k2(new double [m_dim])
  ,m_k3(new double [m_dim])
  ,m_k4(new double [m_dim])
  ,m_k5(new double [m_dim])
  ,m_k6(new double [m_dim])
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "RK5 (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorRK5::~IntegratorRK5()
{
  delete [] m_state;
  delete [] m_tmp;
  delete [] m_k1;
  delete [] m_k2;
  delete [] m_k3;
  delete [] m_k4;
  delete [] m_k5;
  delete [] m_k6;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorRK5::SingleStep()
{
  assert(m_pModel);

  // k1
  m_pModel->Eval(m_state, m_time, m_k1);

  // k2
  for (unsigned i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + 0.25*m_h * m_k1[i];

  m_pModel->Eval(m_tmp, m_time + 0.25*m_h, m_k2);

  // k3
  for (unsigned i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h * (3.0/32.0*m_k1[i] + 9.0/32.0*m_k2[i]);

  m_pModel->Eval(m_tmp, m_time + 3.0/8.0 * m_h, m_k3);

  // k4
  for (unsigned i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h * (1932.0/2197.0*m_k1[i] - 7200.0/2197.0 * m_k2[i] + 7296.0/2197.0*m_k3[i]);

  m_pModel->Eval(m_tmp, m_time + 12.0/13.0 * m_h, m_k4);

  // k5
  for (unsigned i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h * (439.0/216.0*m_k1[i] - 8.0 * m_k2[i] + 3680.0/513.0*m_k3[i] - 845.0/4104.0*m_k4[i]);

  m_pModel->Eval(m_tmp, m_time + m_h, m_k5);

  // K6
  for (unsigned i=0; i<m_dim; ++i)
    m_tmp[i] = m_state[i] + m_h * ( -8.0/27.0*m_k1[i] + 2.0 * m_k2[i] - 3544.0/2565.0*m_k3[i] + 1859.0/4104.0*m_k4[i] - 11.0/40.0*m_k5[i]);

  m_pModel->Eval(m_tmp, m_time + 0.5 * m_h, m_k6);

  // rk4
/*
  for (unsigned i=0; i<m_dim; ++i)
    m_state[i] += m_h * (     25.0 / 216.0  * m_k1[i] +
                            1408.0 / 2565.0 * m_k3[i] +
                            2197.0 / 4104.0 * m_k4[i] -
                               1.0 / 5.0    * m_k5[i]);
   */

  // rk5
  for (unsigned i=0; i<m_dim; ++i)
    m_state[i] += m_h * (     16.0 / 135.0   * m_k1[i] +
                            6656.0 / 12825.0 * m_k3[i] +
                           28561.0 / 56430.0 * m_k4[i] -
                               9.0 / 50.0    * m_k5[i] +
                               2.0 / 55.0    * m_k6[i]);

  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorRK5::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
  {
    m_state[i] = state[i];
    m_tmp[i] = 0;
    m_k1[i] = 0;
    m_k2[i] = 0;
    m_k3[i] = 0;
    m_k4[i] = 0;
    m_k5[i] = 0;
    m_k6[i] = 0;
  }

  m_time = 0;
}

//------------------------------------------------------------------------------
double* IntegratorRK5::GetState() const
{
  return m_state;
}


