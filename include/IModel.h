#ifndef _IMODEL_HPP
#define	_IMODEL_HPP

#include <string>

//----------------------------------------------------------------------------
/** \brief Interface for the models to be simulated. */
class IModel
{
public:

    IModel(const std::string &sName, unsigned dim=1);
    virtual ~IModel();

    unsigned GetDim() const;
    void SetDim(unsigned dim) ;

    std::string GetName() const;

    virtual void Eval(double *state, double time, double *deriv_in) = 0;
    virtual bool IsFinished(double *state) = 0;
    virtual double* GetInitialState() = 0;

private:
    unsigned m_dim;
    std::string m_sName;
};

#endif	/* _IMODEL_HPP */

