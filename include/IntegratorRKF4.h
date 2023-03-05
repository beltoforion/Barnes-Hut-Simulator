/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATOR_RKF4_H
#define	_INTEGRATOR_RKF4_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorRKF4 : public IIntegrator
{
public:

  IntegratorRKF4(IModel *pModel, double h);
  virtual ~IntegratorRKF4();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

  void SetMaximumError(double err);

private:

  double *m_state;
  double *m_tmp;
  double *m_k1;
  double *m_k2;
  double *m_k3;
  double *m_k4;
  double *m_k5;
  double *m_k6;
  double *m_error;
  double m_maxErr;
};

#endif	/* _INTEGRATORHEUN_H */

