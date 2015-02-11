/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_AD24_H
#define	_INTEGRATOR_ADB2_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorADB2 : public IIntegrator
{
public:

  IntegratorADB2(IModel *pModel, double h);
  virtual ~IntegratorADB2();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_f[2];
};

#endif	/* _INTEGRATORHEUN_H */

