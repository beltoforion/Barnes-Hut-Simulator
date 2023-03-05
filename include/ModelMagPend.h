/* 
 * File:   ModelOscillation.hpp
 * Author: user
 *
 * Created on 29. April 2009, 23:09
 */

#ifndef MODEL_MAGPEND_H
#define	MODEL_MAGPEND_H

//--- Standard includes -----------------------------------------------
#include <vector>

//---------------------------------------------------------------------
#include "IModel.h"
#include "Types.h"


//---------------------------------------------------------------------
class ModelMagPend : public IModel
{
public:

  //-----------------------------------------------------------------
  struct Source
  {
    Source(double x,
           double y,
           double z,
           double r,
           double s,
           int type,
           const gfx::Color &col);

    double x, y, z, r, s;
    gfx::Color col;
    int type;
  };

  typedef std::vector<Source> src_vec;

  ModelMagPend(double rad);
  virtual void Eval(double *state, double time, double *deriv) ;
  virtual bool IsFinished(double *state);

  int GetMagNum() const;
  int GetStopIdx() const;
  const src_vec& GetSources() const;

private:

  double m_rk;
  double m_k;
  src_vec m_magnets;
  int m_nIdxMag;
  bool m_bAbort;
};

#endif	/* _MODELOSCILLATION_HPP */

