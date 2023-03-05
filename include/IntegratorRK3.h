/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_RK3_H
#define	_INTEGRATOR_RK3_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorRK3 : public IIntegrator
{
public:

  IntegratorRK3(IModel *pModel, double h);
  virtual ~IntegratorRK3();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_k1;
  double *m_k2;
  double *m_k3;
};

#endif	/* _INTEGRATORHEUN_H */

