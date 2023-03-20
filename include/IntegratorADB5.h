#ifndef _INTEGRATOR_ADB5_H
#define _INTEGRATOR_ADB5_H

#include "IIntegrator.h"
#include "IntegratorRK4.h"

/** \brief Implementation of the ADB5 integration scheme. */
class IntegratorADB5 final : public IIntegrator
{
public:
    IntegratorADB5(IModel *pModel, double h);
    virtual ~IntegratorADB5();
    virtual void SingleStep() override;
    virtual void SetInitialState(double *state) override;
    virtual double *GetState() const override;

private:
    double *_state;
    double *_f[6];
    double _c[6];
    IntegratorRK4 _rk4;
};

#endif // _INTEGRATOR_ADB5_H
