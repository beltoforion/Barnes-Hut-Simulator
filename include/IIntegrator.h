#ifndef _IINTEGRATOR_H
#define	_IINTEGRATOR_H

#include <memory>

#include "IModel.h"


/** \brief Base class for a variety of integration algorithms. */
class IIntegrator
{
public:
  
    IIntegrator(IModel *pModel, double h);
    virtual ~IIntegrator();
    void SetStepSize(double h);
    double GetStepSize() const;
    double GetTime() const;
    double GetError() const;
    IModel* GetModel() const;
    void SetModel(IModel *pModel);

    virtual void Reverse();
    virtual void SetInitialState(double *state) = 0;
    virtual void SingleStep() = 0;
    virtual double* GetState() const = 0;

    const std::string& GetID() const;

protected:

    void Evaluate(const double *initial,  // initial state vector
                  const double *deriv_in, // derivation k input
                  double h,               // step size
                  double time,            // absolute time
                  double *deriv_out);     // the new derivation at h*deriv_in

    void SetID(const std::string &sID);

    IModel *m_pModel;               ///< Pointer to the model implementation
    double m_h;                     ///< The timestep size
    double m_time;                  ///< The absolute time
    double m_err;                   ///< The local error if supported by the integration scheme
    std::string m_sID;

private:

    IIntegrator(const IIntegrator &ref);
    IIntegrator& operator=(const IIntegrator &ref);
};

#endif	// _IINTEGRATOR_H
