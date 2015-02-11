/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_ADB6_H
#define	_INTEGRATOR_ADB6_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorADB6 : public IIntegrator
{
public:

  IntegratorADB6(IModel *pModel, double h);
  virtual ~IntegratorADB6();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;
  virtual void Reverse();

private:

  double *m_state;
  double *m_f[6];
  double m_c[6];
  int m_steps;
};

#endif	/* _INTEGRATORHEUN_H */

