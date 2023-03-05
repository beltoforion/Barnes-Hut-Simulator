/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_ADB4_H
#define	_INTEGRATOR_ADB4_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorADB4 : public IIntegrator
{
public:

  IntegratorADB4(IModel *pModel, double h);
  virtual ~IntegratorADB4();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_f[4];
};

#endif	/* _INTEGRATORHEUN_H */

