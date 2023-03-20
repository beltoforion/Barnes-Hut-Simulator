#ifndef _INTEGRATOR_ADB6_H
#define _INTEGRATOR_ADB6_H

#include <vector>
#include "IIntegrator.h"


/** \brief Implementation of the ADB6 integration scheme. */
class IntegratorADB6 final : public IIntegrator
{
public:
    IntegratorADB6(IModel *pModel, double h);
    virtual ~IntegratorADB6();
    virtual void SingleStep() override;
    virtual void SetInitialState(const double *state) override;
    virtual void Reverse() override;

private:
    double *_f[6];
    double _c[6];
};

#endif // _INTEGRATOR_ADB6_H
