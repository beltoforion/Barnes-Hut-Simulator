/* 
 * File:   integratorheun.cpp
 * Author: user
 * 
 * Created on 3. Mai 2009, 23:40
 */

#include "IntegratorRKF4.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <iostream>


//------------------------------------------------------------------------------
IntegratorRKF4::IntegratorRKF4(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state(new double [m_dim])
  ,m_tmp(new double [m_dim])
  ,m_k1(new double [m_dim])
  ,m_k2(new double [m_dim])
  ,m_k3(new double [m_dim])
  ,m_k4(new double [m_dim])
  ,m_k5(new double [m_dim])
  ,m_k6(new double [m_dim])
  ,m_error(new double [m_dim])
  ,m_maxErr(0.005)
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "RKF4 (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorRKF4::~IntegratorRKF4()
{
  delete [] m_state;
  delete [] m_tmp;
  delete [] m_k1;
  delete [] m_k2;
  delete [] m_k3;
  delete [] m_k4;
  delete [] m_k5;
  delete [] m_k6;
  delete [] m_error;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorRKF4::SingleStep()
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
    m_tmp[i] = m_state[i] + m_h * ( -8.0/27.0*m_k1[i] - 2.0 * m_k2[i] - 3544.0/2565.0*m_k3[i] + 1859.0/4104.0*m_k4[i] - 11.0/55.0*m_k5[i]);

  m_pModel->Eval(m_tmp, m_time + 0.5 * m_h, m_k6);

    for (unsigned i=0; i<m_dim; ++i)
      m_state[i] += m_h * (  25.0 /  216.0 * m_k1[i] +
                           1408.0 / 2565.0 * m_k3[i] +
                           2197.0 / 4104.0 * m_k4[i] -
                              1.0 /    5.0 * m_k5[i]);


  // Fehler
  m_err = 0;
  for (unsigned i=0; i<m_dim; ++i)
  {
    double e = m_h * (  1.0/360.0   * m_k1[i] -
                      128.0/4275.0  * m_k3[i] -
                     2197.0/75240.0 * m_k4[i] +
                        1.0/50.0    * m_k5[i] -
                        2.0/55.0    * m_k6[i]);
    m_err += e * e;
  }

  m_err = std::sqrt(m_err);

  double h = m_h * std::pow(m_maxErr / m_err, 0.25);
  if (h>2.0*m_h)
  {
    m_h = h;
  }
  else if (h<m_h)
  {
    m_h = 0.75 * h;
  }
 
  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorRKF4::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
  {
    m_state[i] = state[i];
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
double* IntegratorRKF4::GetState() const
{
  return m_state;
}

//------------------------------------------------------------------------------
void IntegratorRKF4::SetMaximumError(double maxErr)
{
  m_maxErr = maxErr;
}


