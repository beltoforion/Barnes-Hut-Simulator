/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_ADB3_H
#define	_INTEGRATOR_ADB3_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorADB3 : public IIntegrator
{
public:

  IntegratorADB3(IModel *pModel, double h);
  virtual ~IntegratorADB3();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_f[3];
};

#endif	/* _INTEGRATORHEUN_H */

