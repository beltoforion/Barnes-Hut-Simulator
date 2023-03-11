#include "IModel.h"


IModel::IModel(const std::string &sName, unsigned dim)
    :_dim(dim)
{}


IModel::~IModel()
{}


unsigned IModel::GetDim() const
{
    return _dim;
}


void IModel::SetDim(unsigned dim)
{
    _dim = dim;
}


std::string IModel::GetName() const
{
    return _sName;
}
