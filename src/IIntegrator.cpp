#include "IIntegrator.h"

#include <stdexcept>
#include <cassert>

IIntegrator::IIntegrator(IModel *pModel, double h)
    :m_pModel(pModel)
    ,m_h(h)
    ,m_time(0)
    ,m_err(0)
{
    if (pModel == nullptr)
        throw std::runtime_error("Model pointer may not be NULL");

    if (h <= 0)
        throw std::runtime_error("Step size may not be negative or NULL.");
}


IIntegrator::~IIntegrator()
{}


double IIntegrator::GetStepSize() const
{
    return m_h;
}


void IIntegrator::Reverse()
{
    m_h *= -1;
}


IModel *IIntegrator::GetModel() const
{
    return m_pModel;
}


void IIntegrator::SetModel(IModel *pModel)
{
    m_pModel = pModel;
}


/** \brief Set the stepsize of the integrator. */
void IIntegrator::SetStepSize(double h)
{
    m_h = h;
}


void IIntegrator::SetID(const std::string &sID)
{
    m_sID = sID;
}


const std::string &IIntegrator::GetID() const
{
    return m_sID;
}


/** \brief Returns the absolute time. */
double IIntegrator::GetTime() const
{
    return m_time;
}


double IIntegrator::GetError() const
{
    return m_err;
}


/** \brief Evaluate the model function at a certain point in time. */
void IIntegrator::Evaluate(const double *initial,  // initial state vector
                           const double *deriv_in, // derivation k input
                           double h,               // the new intermediary timestep is at h*k
                           double time,            // absolute time
                           double *deriv_out)      // the new derivation at h*deriv_in
{
    assert(m_pModel);

    // estimate state at timestep i+h
    double state[m_pModel->GetDim()];
    for (std::size_t i = 0; i < m_pModel->GetDim(); ++i)
        state[i] = initial[i] + h * deriv_in[i];

    m_pModel->Eval(state, time + h, deriv_out);
}