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
    double *m_state;
    double *m_f[6];
    double m_c[6];
    IntegratorRK4 m_rk4;
};

#endif // _INTEGRATOR_ADB5_H
