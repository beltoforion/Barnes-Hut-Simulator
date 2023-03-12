#ifndef _INTEGRATOR_RK5_H
#define _INTEGRATOR_RK5_H

#include "IIntegrator.h"

/** \brief Implementation of the simple Euler integration scheme. */
class IntegratorRK5 final : public IIntegrator
{
public:
    IntegratorRK5(IModel *pModel, double h);
    virtual ~IntegratorRK5();
    virtual void SingleStep() override;
    virtual void SetInitialState(double *state) override;
    virtual double *GetState() const override;

private:
    double *_state;
    double *_tmp;
    double *_k1;
    double *_k2;
    double *_k3;
    double *_k4;
    double *_k5;
    double *_k6;
};

#endif // _INTEGRATOR_RK5_H
