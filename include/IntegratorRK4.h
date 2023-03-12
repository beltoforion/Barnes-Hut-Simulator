#ifndef _INTEGRATOR_RK4_H
#define _INTEGRATOR_RK4_H

#include "IIntegrator.h"


/** \brief Implementation of the Runge-Kutta 4th order integration scheme. */
class IntegratorRK4 final : public IIntegrator
{
public:
    IntegratorRK4(IModel *pModel, double h);
    virtual ~IntegratorRK4();

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
};

#endif // _INTEGRATOR_RK4_H