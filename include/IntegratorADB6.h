#ifndef _INTEGRATOR_ADB6_H
#define _INTEGRATOR_ADB6_H

#include "IIntegrator.h"


/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorADB6 final : public IIntegrator
{
public:
    IntegratorADB6(IModel *pModel, double h);
    virtual ~IntegratorADB6();
    virtual void SingleStep() override;
    virtual void SetInitialState(double *state) override;
    virtual double *GetState() const override;
    virtual void Reverse() override;

private:
    double *_state;
    double *_f[6];
    double _c[6];
};

#endif // _INTEGRATOR_ADB6_H
