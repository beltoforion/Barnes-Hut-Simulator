/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_ADB5_H
#define	_INTEGRATOR_ADB5_H

#include "IIntegrator.h"
#include "IntegratorRK4.h"

//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorADB5 : public IIntegrator
{
public:

  IntegratorADB5(IModel *pModel, double h);
  virtual ~IntegratorADB5();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_f[6];
  double m_c[6];
  IntegratorRK4 m_rk4;
  int m_steps;
};

#endif	/* _INTEGRATORHEUN_H */

