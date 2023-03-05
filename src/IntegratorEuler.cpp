#include "IntegratorEuler.h"

//--- Standard includes --------------------------------------------------------
#include <cassert>
#include <stdexcept>
#include <sstream>


//------------------------------------------------------------------------------
IntegratorEuler::IntegratorEuler(IModel *pModel, double h)
  :IIntegrator(pModel, h)
  ,m_state((pModel) ? new double [pModel->GetDim()] : NULL)
{
  if (pModel==NULL)
    throw std::runtime_error("Model pointer may not be NULL.");

  std::stringstream ss;
  ss << "Euler (dt=" << m_h << ")";
  SetID(ss.str());
}

//------------------------------------------------------------------------------
IntegratorEuler::~IntegratorEuler()
{
  delete [] m_state;
}

//------------------------------------------------------------------------------
/** \brief Performs a single integration step. */
void IntegratorEuler::SingleStep()
{
  double k1[m_dim];
  m_pModel->Eval(m_state, m_time, k1);

  for (std::size_t i=0; i<m_dim; ++i)
    m_state[i] += m_h * k1[i];

  m_time += m_h;
}

//------------------------------------------------------------------------------
/** \brief Sets the initial state of the simulation. */
void IntegratorEuler::SetInitialState(double *state)
{
  for (unsigned i=0; i<m_dim; ++i)
    m_state[i] = state[i];

  m_time = 0;
}

//------------------------------------------------------------------------------
double* IntegratorEuler::GetState() const
{
  return m_state;
}
