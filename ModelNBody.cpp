/*
 * File:   ModelMagneticPendulum.cpp
 * Author: user
 *
 * Created on 29. April 2009, 23:08
 */

#include "ModelNBody.h"

//--- Standard includes --------------------------------------------------
#include <cstdlib>
#include <cmath>
#include <limits>
#include <iostream>
#include <omp.h>

using namespace std;


//------------------------------------------------------------------------
ModelNBody::ModelNBody()
  :IModel("N-Body simulation (2D)")
  ,m_pInitial(NULL)
  ,m_pAux(NULL)
  ,m_root(BHTreeNode(Vec2D(), Vec2D()))
  ,m_min()
  ,m_max()
  ,m_center()
  ,m_camDir()
  ,m_camPos()
  ,m_roi(1)
  ,m_timeStep(1)
  ,mass_sun(1.988435e30)
  ,pc_in_m(3.08567758129e16)
  ,gamma_si(6.67428e-11)
  ,gamma_1(gamma_si/(pc_in_m*pc_in_m*pc_in_m)*mass_sun*(365.25*86400)*(365.25*86400))
  ,time_1(sqrt( (pc_in_m*pc_in_m*pc_in_m)/(gamma_si*mass_sun) ) / (365.25*86400))
  ,m_num(0)
  ,m_bVerbose(false)
{
  BHTreeNode::s_gamma = gamma_1;

//  Init();
  InitCollision();
//  Init3Body();
}

//------------------------------------------------------------------------
ModelNBody::~ModelNBody()
{
  delete m_pInitial;
  delete m_pAux;
}

//------------------------------------------------------------------------
void ModelNBody::SetROI(double roi)
{
  m_roi = roi;
}

//------------------------------------------------------------------------
double ModelNBody::GetSuggestedTimeStep() const
{
  return m_timeStep;
}

//------------------------------------------------------------------------
double ModelNBody::GetROI() const
{
  return m_roi;
}

//------------------------------------------------------------------------
Vec3D ModelNBody::GetCenterOfMass() const
{
  const Vec2D &cm2d = m_root.GetCenterOfMass();
  return Vec3D(cm2d.x, cm2d.y, 0);
}

//------------------------------------------------------------------------
const Vec3D& ModelNBody::GetCamDir() const
{
  return m_camDir;
}

//------------------------------------------------------------------------
const Vec3D& ModelNBody::GetCamPos() const
{
  return m_camPos;
}

//------------------------------------------------------------------------
double* ModelNBody::GetInitialState()
{
  return reinterpret_cast<double*>(m_pInitial);
}

//------------------------------------------------------------------------
void ModelNBody::GetOrbitalVelocity(const ParticleData &p1, const ParticleData &p2)
{
  double x1 = p1.m_pState->x,
         y1 = p1.m_pState->x,
         m1 = p1.m_pAuxState->mass;
  double x2 = p2.m_pState->x,
         y2 = p2.m_pState->y;

  // Calculate distance from the planet with index idx_main
  double r[2], dist;
  r[0] = x1 - x2;
  r[1] = y1 - y2;

  // distance in parsec
  dist = sqrt(r[0] * r[0] + r[1] * r[1]);

  // Based on the distance from the sun calculate the velocity needed to maintain a circular orbit
  double v = sqrt(gamma_1 * m1 / dist);

  // Calculate a suitable vector perpendicular to r for the velocity of the tracer
  double &vx = p2.m_pState->vx,
         &vy = p2.m_pState->vy;
  vx = ( r[1] / dist) * v;
  vy = (-r[0] / dist) * v;
}

//------------------------------------------------------------------------
void ModelNBody::ResetDim(int num, double stepsize)
{
  m_num = num;
  SetDim(m_num*4);

  delete m_pInitial;
  m_pInitial = new PODState[num];

  delete m_pAux;
  m_pAux = new PODAuxState[num];

  m_timeStep = stepsize;

  // reset bounding box and center
  m_max.x = m_max.y = std::numeric_limits<double>::min();
  m_min.x = m_min.y = std::numeric_limits<double>::max();
  m_center = Vec2D(0,0);    // for storing the center of mass
}

//------------------------------------------------------------------------
void ModelNBody::Init()
{
  // Reset model size
  ResetDim(4000, 100000);

  double mass = 0;  // for storing the total mass

  // initialize particles
  int ct = 0;
  ParticleData blackHole, macho[10];

  for (int k=0; k<40; ++k)
  {
    for (int l=0; l<100; ++l)
    {
      if (ct>=m_num)
        goto hell;

      PODState &st        = m_pInitial[ct];
      PODAuxState &st_aux = m_pAux[ct];

      if (ct==0)
      {
        blackHole.m_pState = &st;
        blackHole.m_pAuxState = &st_aux;

        // particle zero is special its the trace particle that is not part
        // of the simulation and can be positioned with the mouse
        st.x  = st.y = 0;
        st.vx = st.vy = 0;
        st_aux.mass = 1000000; //431000;   // 4.31 Millionen Sonnenmassen
      }
      else if (ct==1)
      {
        // macho im galaktischen halo, der hoffentlich ein paar spiralarme erzeugt
        macho[0].m_pState = &st;
        macho[0].m_pAuxState = &st_aux;

        // particle zero is special its the trace particle that is not part
        // of the simulation and can be positioned with the mouse
        st_aux.mass = blackHole.m_pAuxState->mass / 10.0;
        st.x = 5000;
        st.y = 5000;

        GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
      }
      else if (ct==2)
      {
        // macho im galaktischen halo, der hoffentlich ein paar spiralarme erzeugt
        macho[1].m_pState = &st;
        macho[1].m_pAuxState = &st_aux;

        // particle zero is special its the trace particle that is not part
        // of the simulation and can be positioned with the mouse
        st_aux.mass = blackHole.m_pAuxState->mass / 10.0;
        st.x = -5000;
        st.y = -5000;

        GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
      }
      else
      {
        st_aux.mass = 0.76 + 100 * ((double)rand() / RAND_MAX);
        double rad = 1200 + k*100;
        st.x = rad*sin(2*M_PI * l/100.0);
        st.y = rad*cos(2*M_PI * l/100.0);
        GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
      }

      // determine the size of the area including all particles
      m_max.x = std::max(m_max.x, st.x);
      m_max.y = std::max(m_max.y, st.y);
      m_min.x = std::min(m_min.x, st.x);
      m_min.y = std::min(m_min.y, st.y);

      m_center.x += st.x * st_aux.mass;
      m_center.y += st.x * st_aux.mass;
      mass += st_aux.mass;
      ++ct;
    }

  }

  hell:

  // compute the center of mass
  m_center.x /= mass;
  m_center.y /= mass;

  // The Barnes Hut algorithm needs square shaped quadrants.
  // calculate the height of the square including all particles (and a bit more space)
  m_roi  = 1.5 * std::max(m_max.x - m_min.x,
                          m_max.y - m_min.y);

  // compute the center of the region including all particles
  m_min.x = m_center.x - m_roi;
  m_max.x = m_center.x + m_roi;
  m_min.y = m_center.y - m_roi;
  m_max.y = m_center.y + m_roi;

  std::cout << "Initial particle distribution area\n";
  std::cout << "----------------------------------\n";
  std::cout << "Particle spread:\n";
  std::cout << "  xmin   = " << m_min.x << ", ymin=" << m_min.y << "\n";
  std::cout << "  xmax   = " << m_max.y << ", ymax=" << m_max.y << "\n";
  std::cout << "Bounding box:\n";
  std::cout << "  center = " << m_center.x   << ", cy  =" << m_center.y   << "\n";
  std::cout << "  roi    = " << m_roi << "\n";
}


//------------------------------------------------------------------------
void ModelNBody::InitCollision()
{
  // Reset model size
  ResetDim(5000, 100);

  // initialize particles
  ParticleData blackHole;
  ParticleData blackHole2;

  for (int i=0; i<m_num; ++i)
  {
    PODState &st        = m_pInitial[i];
    PODAuxState &st_aux = m_pAux[i];

    if (i==0)
    {
      // particle zero is special its the trace particle that is not part
      // of the simulation and can be positioned with the mouse
      blackHole.m_pState = &st;
      blackHole.m_pAuxState = &st_aux;

      st.x  = st.y = 0;
      st.vx = st.vy = 0;
      st_aux.mass = 1000000; //431000;   // 4.31 Millionen Sonnenmassen
    }
    else if (i<4000)
    {
      const double rad = 10;
      double r = 0.1 + .8 * (rad * ((double)rand() / RAND_MAX));
      double a = 2.0*M_PI*((double)rand() / RAND_MAX);
      st_aux.mass = 0.03 + 20 * ((double)rand() / RAND_MAX);
      st.x = r*sin(a);
      st.y = r*cos(a);

      GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
    }
    else if (i==4000)
    {
      blackHole2.m_pState = &st;
      blackHole2.m_pAuxState = &st_aux;

      st.x = st.y = 10;
      st_aux.mass = 100000;
      GetOrbitalVelocity(blackHole, blackHole2);
      blackHole2.m_pState->vx *= 0.9;
      blackHole2.m_pState->vy *= 0.9;
    }
    else
    {
      const double rad = 3;
      double r = 0.1 + .8 *  (rad * ((double)rand() / RAND_MAX));
      double a = 2.0*M_PI*((double)rand() / RAND_MAX);
      st_aux.mass = 0.03 + 20 * ((double)rand() / RAND_MAX);
      st.x = blackHole2.m_pState->x + r*sin(a);
      st.y = blackHole2.m_pState->y + r*cos(a);

      GetOrbitalVelocity(blackHole2, ParticleData(&st, &st_aux));
      st.vx+=blackHole2.m_pState->vx;
      st.vy+=blackHole2.m_pState->vy;
    }

    // determine the size of the area including all particles
    m_max.x = std::max(m_max.x, st.x);
    m_max.y = std::max(m_max.y, st.y);
    m_min.x = std::min(m_min.x, st.x);
    m_min.y = std::min(m_min.y, st.y);
  }

  // The Barnes Hut algorithm needs square shaped quadrants.
  // calculate the height of the square including all particles (and a bit more space)
  double l = 1.05 * std::max(m_max.x - m_min.x,
                             m_max.y - m_min.y);

  m_roi = l * 1.5;

  // compute the center of the region including all particles
  Vec2D c(m_min.x + (m_max.x - m_min.x)/2.0,
          m_min.y + (m_max.y - m_min.y)/2.0);
  m_min.x = c.x - l/2.0;
  m_max.x = c.x + l/2.0;
  m_min.y = c.y - l/2.0;
  m_max.y = c.y + l/2.0;

  std::cout << "Initial particle distribution area\n";
  std::cout << "----------------------------------\n";
  std::cout << "Particle spread:\n";
  std::cout << "  xmin=" << m_min.x << ", ymin=" << m_min.y << "\n";
  std::cout << "  xmax=" << m_max.y << ", ymax=" << m_max.y << "\n";
  std::cout << "Bounding box:\n";
  std::cout << "  cx =" << c.x   << ", cy  =" << c.y   << "\n";
  std::cout << "  l  =" << l << "\n";
}

//------------------------------------------------------------------------------
void ModelNBody::Init3Body()
{
  // Reset model size
  ResetDim(3, .5);
  m_root.SetTheta(0.9);
  PODState *st(NULL);
  PODAuxState *st_aux(NULL);

  // initialize particles
  st     = &m_pInitial[0];
  st_aux = &m_pAux[0];
  st->x  = 1;
  st->y  = 3;
  st->vx = st->vy = 0;
  st_aux->mass = 3;

  st     = &m_pInitial[1];
  st_aux = &m_pAux[1];
  st->x  = -2;
  st->y  = -1;
  st->vx = st->vy = 0;
  st_aux->mass = 4;

  st     = &m_pInitial[2];
  st_aux = &m_pAux[2];
  st->x   =  1;
  st->y   = -1;
  st->vx  = st->vy = 0;
  st_aux->mass = 5;

  // determine the size of the area including all particles
  for (int i=0; i<m_num; ++i)
  {
    PODState &st        = m_pInitial[i];
    m_max.x = std::max(m_max.x, st.x);
    m_max.y = std::max(m_max.y, st.y);
    m_min.x = std::min(m_min.x, st.x);
    m_min.y = std::min(m_min.y, st.y);
  }

  // The Barnes Hut algorithm needs square shaped quadrants.
  // calculate the height of the square including all particles (and a bit more space)
  double l = 1.05 * std::max(m_max.x - m_min.x,
                             m_max.y - m_min.y);

  m_roi = l * 1.5;

  // compute the center of the region including all particles
  Vec2D c(m_min.x + (m_max.x - m_min.x)/2.0,
          m_min.y + (m_max.y - m_min.y)/2.0);
  m_min.x = c.x - l/2.0;
  m_max.x = c.x + l/2.0;
  m_min.y = c.y - l/2.0;
  m_max.y = c.y + l/2.0;

  std::cout << "Initial particle distribution area\n";
  std::cout << "----------------------------------\n";
  std::cout << "Particle spread:\n";
  std::cout << "  xmin=" << m_min.x << ", ymin=" << m_min.y << "\n";
  std::cout << "  xmax=" << m_max.y << ", ymax=" << m_max.y << "\n";
  std::cout << "Bounding box:\n";
  std::cout << "  cx =" << c.x   << ", cy  =" << c.y   << "\n";
  std::cout << "  l  =" << l << "\n";
}

//------------------------------------------------------------------------------
void ModelNBody::CalcBHArea(const ParticleData &data)
{
/*
  // reset bounding box
  m_max.x = m_max.y = std::numeric_limits<double>::min();
  m_min.x = m_min.y = std::numeric_limits<double>::max();

  for (int i=0; i<m_num; ++i)
  {
    PODState &s = data.m_pState[i];

    // determine the size of the area including all particles
    m_max.x = std::max(m_max.x, s.x);
    m_max.y = std::max(m_max.y, s.y);
    m_min.x = std::min(m_min.x, s.x);
    m_min.y = std::min(m_min.y, s.y);
  }

  // The Barnes Hut algorithm needs square shaped quadrants.
  // calculate the height of the square including all particles (and a bit more space)
  double l = 1.05 * std::max(m_max.x - m_min.x,
                             m_max.y - m_min.y);

  // compute the center of the region including all particles
  Vec2D c(m_min.x + (m_max.x - m_min.x)/2.0,
          m_min.y + (m_max.y - m_min.y)/2.0);
  m_min.x = c.x - l/2.0;
  m_max.x = c.x + l/2.0;
  m_min.y = c.y - l/2.0;
  m_max.y = c.y + l/2.0;
*/
}

//------------------------------------------------------------------------------
/** \brief Build the barnes hut tree by adding all particles that are inside
           the region of interest.
*/
void ModelNBody::BuiltTree(const ParticleData &all)
{
  // Reset the quadtree, make sure only particles inside the roi
  // are handled. The renegade ones may live long and prosper
  // outside my simulation
  m_root.Reset(Vec2D(m_center.x - m_roi, m_center.y - m_roi),
               Vec2D(m_center.x + m_roi, m_center.y + m_roi));

  // build the quadtree
  int ct = 0;
  for (int i=0; i<m_num; ++i)
  {
//    PODState *st = &(all.m_pState[i]);

    try
    {
      // extract data for a single particle
      ParticleData p(&(all.m_pState[i]),
                     &(all.m_pAuxState[i]));

      // insert the particle, but only if its inside the roi
      m_root.Insert(p, 0);
      ++ct;
    }
    catch(std::exception &exc)
    {
/*
      std::cout << exc.what() << "\n";
      std::cout << "Particle " << i << " (" << st->x << ", " << st->y << ") is outside the roi (skipped).\n";
      std::cout << "  roi size   =   " << m_roi << "\n";
      std::cout << "  roi center = (" << m_center.x << ", " << m_center.y << ")\n";
*/
    }
  }

//  std::cout << ct << " particles added sucessfully\n";

  // compute masses and center of mass on all scales of the tree
  m_root.ComputeMassDistribution();
  if (m_bVerbose)
  {
    std::cout << "Tree Dump\n";
    std::cout << "---------\n";
    m_root.DumpNode(-1, 0);
    std::cout << "\n\n";
  }

  // update the center of mass
  m_center = m_root.GetCenterOfMass();
}

//------------------------------------------------------------------------
const PODAuxState* ModelNBody::GetAuxState() const
{
  return m_pAux;
}

//------------------------------------------------------------------------
BHTreeNode* ModelNBody::GetRootNode()
{
  return &m_root;
}

//------------------------------------------------------------------------
int ModelNBody::GetN() const
{
  return m_num;
}

//------------------------------------------------------------------------
double ModelNBody::GetTheta() const
{
  return m_root.GetTheta();
}

//------------------------------------------------------------------------
void ModelNBody::SetVerbose(bool bVerbose)
{
  m_bVerbose = bVerbose;
}

//------------------------------------------------------------------------
void ModelNBody::SetTheta(double theta)
{
  m_root.SetTheta(theta);
}

//------------------------------------------------------------------------
double ModelNBody::GetTimeUnit() const
{
  return time_1;
}

//------------------------------------------------------------------------
void ModelNBody::Eval(double *a_state, double a_time, double *a_deriv)
{
  // wrap the complete particle data together for easier treatment
  // in the following algorithms
  PODState *pState = reinterpret_cast<PODState*>(a_state);
  PODDeriv *pDeriv = reinterpret_cast<PODDeriv*>(a_deriv);
  ParticleData all(pState, m_pAux);

  CalcBHArea(all);
  BuiltTree(all);

  #pragma omp parallel for
  for (int i=1; i<m_num; ++i)
  {
    ParticleData p(&pState[i], &m_pAux[i]);
    Vec2D acc = m_root.CalcForce(p);
    pDeriv[i].ax = acc.x;
    pDeriv[i].ay = acc.y;
    pDeriv[i].vx = pState[i].vx;
    pDeriv[i].vy = pState[i].vy;
  }

  // Particle 0 is calculated last, because the statistics
  // data relate to this particle. They would be overwritten
  // otherwise
  m_root.StatReset();
  ParticleData p(&pState[0], &m_pAux[0]);
  Vec2D acc = m_root.CalcForce(p);
  pDeriv[0].ax = acc.x;
  pDeriv[0].ay = acc.y;
  pDeriv[0].vx = pState[0].vx;
  pDeriv[0].vy = pState[0].vy;


  // Save vectors for camera orientations
//  m_camDir.x = pState[0].x - pState[4000].x;
//  m_camDir.y = pState[0].y - pState[4000].y;
  m_camPos.x = m_root.GetCenterOfMass().x;
  m_camPos.y = m_root.GetCenterOfMass().y;
}

//------------------------------------------------------------------------
bool ModelNBody::IsFinished(double *state)
{
  return false;
}
