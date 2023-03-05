#include "IModel.h"

//---------------------------------------------------------------------
IModel::IModel(const std::string &sName, unsigned dim)
  :m_dim(dim)
{}

//---------------------------------------------------------------------
IModel::~IModel()
{}

//---------------------------------------------------------------------
unsigned IModel::GetDim() const
{
  return m_dim;
}

//---------------------------------------------------------------------
void IModel::SetDim(unsigned dim)
{
  m_dim = dim;
}

//---------------------------------------------------------------------
std::string IModel::GetName() const
{
  return m_sName;
}
