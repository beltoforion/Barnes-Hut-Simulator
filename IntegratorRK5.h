/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_RK5_H
#define	_INTEGRATOR_RK5_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorRK5 : public IIntegrator
{
public:

  IntegratorRK5(IModel *pModel, double h);
  virtual ~IntegratorRK5();
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
  double *m_k5;
  double *m_k6;
};

#endif	/* _INTEGRATORHEUN_H */

