/* 
 * File:   integratorheun.h
 * Author: user
 *
 * Created on 3. Mai 2009, 23:40
 */

#ifndef _INTEGRATORHEUN_H
#define	_INTEGRATORHEUN_H

#include "IIntegrator.h"


//------------------------------------------------------------------------------
/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorHeun : public IIntegrator
{
public:

  IntegratorHeun(IModel *pModel, double h);
  virtual ~IntegratorHeun();
  virtual void SingleStep();
  virtual void SetInitialState(double *state);
  virtual double* GetState() const;

private:

  double *m_state;
  double *m_tmp;
  double *m_k1;
  double *m_k2;
};

#endif	/* _INTEGRATORHEUN_H */

