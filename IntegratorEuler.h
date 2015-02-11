#ifndef _INTEGRATOR_IMPL_H
#define _INTEGRATOR_IMPL_H

//------------------------------------------------------------------------------
#include <string>

//------------------------------------------------------------------------------
#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorEuler : public IIntegrator
{
public:

  IntegratorEuler(IModel *pModel, double h);
  virtual ~IntegratorEuler();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
};


#endif // include guard