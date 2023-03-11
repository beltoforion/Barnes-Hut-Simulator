#ifndef _INTEGRATOR_RK5_H
#define _INTEGRATOR_RK5_H

#include "IIntegrator.h"

/** \brief Inplementation of the simple Euler integration scheme. */
class IntegratorRK5 final : public IIntegrator
{
public:
    IntegratorRK5(IModel *pModel, double h);
    virtual ~IntegratorRK5();
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
    double *m_k5;
    double *m_k6;
};

#endif // _INTEGRATOR_RK5_H
