#ifndef _INTEGRATOR_ADB4_H
#define _INTEGRATOR_ADB4_H

#include "IIntegrator.h"


/** \brief Implementation of the ADB 4 integration scheme. */
class IntegratorADB4 final : public IIntegrator
{
public:
    IntegratorADB4(IModel *pModel, double h);
    virtual ~IntegratorADB4();
    virtual void SingleStep() override;
    virtual void SetInitialState(double *state) override;
    virtual double *GetState() const override;

private:
    double *_state;
    double *_f[4];
};

#endif // _INTEGRATOR_ADB4_H
