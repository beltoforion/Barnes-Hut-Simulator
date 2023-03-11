#ifndef _INTEGRATOR_RK4_H
#define _INTEGRATOR_RK4_H

#include "IIntegrator.h"


/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorRK4 final : public IIntegrator
{
public:
    IntegratorRK4(IModel *pModel, double h);
    virtual ~IntegratorRK4();

    virtual void SingleStep() override;
    virtual void SetInitialState(double *state) override;
    virtual double *GetState() const override;

private:
    double *m_state;
    double *m_tmp;
    double *m_k1;
    double *m_k2;
    double *m_k3;
    double *m_k4;
};

#endif // _INTEGRATOR_RK4_H