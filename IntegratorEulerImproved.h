/* 
 * File:   integratoreulerimproved.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:24
 */

#ifndef _INTEGRATOREULERIMPROVED_H
#define	_INTEGRATOREULERIMPROVED_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorEulerImproved : public IIntegrator
{
public:

  IntegratorEulerImproved(IModel *pModel, double h);
  virtual ~IntegratorEulerImproved();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_tmp;
  double *m_k1;
  double *m_k2;
};

#endif	/* _INTEGRATOREULERIMPROVED_H */

