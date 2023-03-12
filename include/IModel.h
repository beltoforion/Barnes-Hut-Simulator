#ifndef _IMODEL_HPP
#define	_IMODEL_HPP

#include <string>


/** \brief Interface for the models to be simulated. */
class IModel
{
public:

    IModel(const std::string &name, unsigned dim=1)
        :_dim(dim)
        ,_name(name)
    {}

    virtual ~IModel() {}

    unsigned GetDim() const noexcept { return _dim; }   
    void SetDim(unsigned dim) noexcept { _dim = dim; }

    std::string GetName() const noexcept { return _name; }

    virtual void Eval(double *state, double time, double *deriv_in) = 0;
    virtual bool IsFinished(double *state) = 0;
    virtual double* GetInitialState() = 0;

private:
    unsigned _dim;
    std::string _name;
};

#endif	// _IMODEL_HPP

