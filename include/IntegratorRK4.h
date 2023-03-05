/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_RK4_H
#define	_INTEGRATOR_RK4_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorRK4 : public IIntegrator
{
public:

  IntegratorRK4(IModel *pModel, double h);
  virtual ~IntegratorRK4();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_tmp;
  double *m_k1;
  double *m_k2;
  double *m_k3;
  double *m_k4;
};

#endif	/* _INTEGRATORHEUN_H */

