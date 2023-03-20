#ifndef _INTEGRATOR_RKF4_H
#define _INTEGRATOR_RKF4_H

#include "IIntegrator.h"

/** \brief Implementation of the simple Euler integration scheme. */
class IntegratorRKF4 final : public IIntegrator
{
public:
    IntegratorRKF4(IModel *pModel, double h);
    virtual ~IntegratorRKF4();
    virtual void SingleStep() override;
    virtual void SetInitialState(double *state) override;
    virtual double *GetState() const override;

    void SetMaximumError(double err);

private:
    double *_state;
    double *_tmp;
    double *_k1;
    double *_k2;
    double *_k3;
    double *_k4;
    double *_k5;
    double *_k6;
    double *_error;
    double _maxErr;
};

#endif // _INTEGRATOR_RKF4_H
