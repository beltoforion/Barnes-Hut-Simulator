#include "ModelNBody.h"

//--- Standard includes --------------------------------------------------
#include <cstdlib>
#include <cmath>
#include <limits>
#include <iostream>
#include <omp.h>

using namespace std;


ModelNBody::ModelNBody()
    :IModel("N-Body simulation (2D)")
    ,_pInitial(nullptr)
    ,_pAux(nullptr)
    ,_root(BHTreeNode(Vec2D(), Vec2D()))
    ,_min()
    ,_max()
    ,_center()
    ,_camDir()
    ,_camPos()
    ,_roi(1)
    ,_timeStep(1)
    ,_num(0)
    ,_bVerbose(false)
{
    BHTreeNode::s_gamma = gamma_1;

    //  Init();
    InitCollision();
    //  Init3Body();
}


ModelNBody::~ModelNBody()
{
    delete _pInitial;
    delete _pAux;
}


void ModelNBody::SetROI(double roi)
{
    _roi = roi;
}


double ModelNBody::GetSuggestedTimeStep() const
{
    return _timeStep;
}


double ModelNBody::GetROI() const
{
    return _roi;
}


Vec3D ModelNBody::GetCenterOfMass() const
{
    const Vec2D &cm2d = _root.GetCenterOfMass();
    return Vec3D(cm2d.x, cm2d.y, 0);
}


const Vec3D &ModelNBody::GetCamDir() const
{
    return _camDir;
}


const Vec3D &ModelNBody::GetCamPos() const
{
    return _camPos;
}


double *ModelNBody::GetInitialState()
{
    return reinterpret_cast<double *>(_pInitial);
}


void ModelNBody::GetOrbitalVelocity(const ParticleData &p1, const ParticleData &p2)
{
    double x1 = p1._pState->x,
           y1 = p1._pState->x,
           m1 = p1._pAuxState->mass;
    double x2 = p2._pState->x,
           y2 = p2._pState->y;

    // Calculate distance from the planet with index idx_main
    double r[2], dist;
    r[0] = x1 - x2;
    r[1] = y1 - y2;

    // distance in parsec
    dist = sqrt(r[0] * r[0] + r[1] * r[1]);

    // Based on the distance from the sun calculate the velocity needed to maintain a circular orbit
    double v = sqrt(gamma_1 * m1 / dist);

    // Calculate a suitable vector perpendicular to r for the velocity of the tracer
    double &vx = p2._pState->vx,
           &vy = p2._pState->vy;
    vx = (r[1] / dist) * v;
    vy = (-r[0] / dist) * v;
}


void ModelNBody::ResetDim(int num, double stepsize)
{
    _num = num;
    SetDim(_num * 4);

    delete _pInitial;
    _pInitial = new PODState[num];

    delete _pAux;
    _pAux = new PODAuxState[num];

    _timeStep = stepsize;

    // reset bounding box and center
    _max.x = _max.y = std::numeric_limits<double>::min();
    _min.x = _min.y = std::numeric_limits<double>::max();
    _center = Vec2D(0, 0); // for storing the center of mass
}


void ModelNBody::Init()
{
    // Reset model size
    ResetDim(5000, 100000);

    double mass = 0; // for storing the total mass

    // initialize particles
    int ct = 0;
    ParticleData blackHole, macho[10];

    for (int k = 0; k < 40; ++k)
    {
        for (int l = 0; l < 100; ++l)
        {
            if (ct >= _num)
                goto hell;

            PODState &st = _pInitial[ct];
            PODAuxState &st_aux = _pAux[ct];

            if (ct == 0)
            {
                blackHole._pState = &st;
                blackHole._pAuxState = &st_aux;

                // particle zero is special its the trace particle that is not part
                // of the simulation and can be positioned with the mouse
                st.x = st.y = 0;
                st.vx = st.vy = 0;
                st_aux.mass = 1000000; // 431000;   // 4.31 Millionen Sonnenmassen
            }
            else if (ct == 1)
            {
                // macho im galaktischen halo, der hoffentlich ein paar spiralarme erzeugt
                macho[0]._pState = &st;
                macho[0]._pAuxState = &st_aux;

                // particle zero is special its the trace particle that is not part
                // of the simulation and can be positioned with the mouse
                st_aux.mass = blackHole._pAuxState->mass / 10.0;
                st.x = 5000;
                st.y = 5000;

                GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
            }
            else if (ct == 2)
            {
                // macho im galaktischen halo, der hoffentlich ein paar spiralarme erzeugt
                macho[1]._pState = &st;
                macho[1]._pAuxState = &st_aux;

                // particle zero is special its the trace particle that is not part
                // of the simulation and can be positioned with the mouse
                st_aux.mass = blackHole._pAuxState->mass / 10.0;
                st.x = -5000;
                st.y = -5000;

                GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
            }
            else
            {
                st_aux.mass = 0.76 + 100 * ((double)rand() / RAND_MAX);
                double rad = 1200 + k * 100;
                st.x = rad * sin(2 * M_PI * l / 100.0);
                st.y = rad * cos(2 * M_PI * l / 100.0);
                GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
            }

            // determine the size of the area including all particles
            _max.x = std::max(_max.x, st.x);
            _max.y = std::max(_max.y, st.y);
            _min.x = std::min(_min.x, st.x);
            _min.y = std::min(_min.y, st.y);

            _center.x += st.x * st_aux.mass;
            _center.y += st.x * st_aux.mass;
            mass += st_aux.mass;
            ++ct;
        }
    }

hell:

    // compute the center of mass
    _center.x /= mass;
    _center.y /= mass;

    // The Barnes Hut algorithm needs square shaped quadrants.
    // calculate the height of the square including all particles (and a bit more space)
    _roi = 1.5 * std::max(_max.x - _min.x,
                           _max.y - _min.y);

    // compute the center of the region including all particles
    _min.x = _center.x - _roi;
    _max.x = _center.x + _roi;
    _min.y = _center.y - _roi;
    _max.y = _center.y + _roi;

    std::cout << "Initial particle distribution area\n";
    std::cout << "----------------------------------\n";
    std::cout << "Particle spread:\n";
    std::cout << "  xmin   = " << _min.x << ", ymin=" << _min.y << "\n";
    std::cout << "  xmax   = " << _max.y << ", ymax=" << _max.y << "\n";
    std::cout << "Bounding box:\n";
    std::cout << "  center = " << _center.x << ", cy  =" << _center.y << "\n";
    std::cout << "  roi    = " << _roi << "\n";
}


void ModelNBody::InitCollision()
{
    // Reset model size
    ResetDim(5000, 100);

    // initialize particles
    ParticleData blackHole;
    ParticleData blackHole2;

    for (int i = 0; i < _num; ++i)
    {
        PODState &st = _pInitial[i];
        PODAuxState &st_aux = _pAux[i];

        if (i == 0)
        {
            // particle zero is special its the trace particle that is not part
            // of the simulation and can be positioned with the mouse
            blackHole._pState = &st;
            blackHole._pAuxState = &st_aux;

            st.x = st.y = 0;
            st.vx = st.vy = 0;
            st_aux.mass = 1000000; // 431000;   // 4.31 Millionen Sonnenmassen
        }
        else if (i < 4000)
        {
            const double rad = 10;
            double r = 0.1 + .8 * (rad * ((double)rand() / RAND_MAX));
            double a = 2.0 * M_PI * ((double)rand() / RAND_MAX);
            st_aux.mass = 0.03 + 20 * ((double)rand() / RAND_MAX);
            st.x = r * sin(a);
            st.y = r * cos(a);

            GetOrbitalVelocity(blackHole, ParticleData(&st, &st_aux));
        }
        else if (i == 4000)
        {
            blackHole2._pState = &st;
            blackHole2._pAuxState = &st_aux;

            st.x = st.y = 10;
            st_aux.mass = 100000;
            GetOrbitalVelocity(blackHole, blackHole2);
            blackHole2._pState->vx *= 0.9;
            blackHole2._pState->vy *= 0.9;
        }
        else
        {
            const double rad = 3;
            double r = 0.1 + .8 * (rad * ((double)rand() / RAND_MAX));
            double a = 2.0 * M_PI * ((double)rand() / RAND_MAX);
            st_aux.mass = 0.03 + 20 * ((double)rand() / RAND_MAX);
            st.x = blackHole2._pState->x + r * sin(a);
            st.y = blackHole2._pState->y + r * cos(a);

            GetOrbitalVelocity(blackHole2, ParticleData(&st, &st_aux));
            st.vx += blackHole2._pState->vx;
            st.vy += blackHole2._pState->vy;
        }

        // determine the size of the area including all particles
        _max.x = std::max(_max.x, st.x);
        _max.y = std::max(_max.y, st.y);
        _min.x = std::min(_min.x, st.x);
        _min.y = std::min(_min.y, st.y);
    }

    // The Barnes Hut algorithm needs square shaped quadrants.
    // calculate the height of the square including all particles (and a bit more space)
    double l = 1.05 * std::max(_max.x - _min.x,
                               _max.y - _min.y);

    _roi = l * 1.5;

    // compute the center of the region including all particles
    Vec2D c(_min.x + (_max.x - _min.x) / 2.0,
            _min.y + (_max.y - _min.y) / 2.0);
    _min.x = c.x - l / 2.0;
    _max.x = c.x + l / 2.0;
    _min.y = c.y - l / 2.0;
    _max.y = c.y + l / 2.0;

    std::cout << "Initial particle distribution area\n";
    std::cout << "----------------------------------\n";
    std::cout << "Particle spread:\n";
    std::cout << "  xmin=" << _min.x << ", ymin=" << _min.y << "\n";
    std::cout << "  xmax=" << _max.y << ", ymax=" << _max.y << "\n";
    std::cout << "Bounding box:\n";
    std::cout << "  cx =" << c.x << ", cy  =" << c.y << "\n";
    std::cout << "  l  =" << l << "\n";
}


void ModelNBody::Init3Body()
{
    // Reset model size
    ResetDim(3, .5);
    _root.SetTheta(0.9);
    PODState *st(nullptr);
    PODAuxState *st_aux(nullptr);

    // initialize particles
    st = &_pInitial[0];
    st_aux = &_pAux[0];
    st->x = 1;
    st->y = 3;
    st->vx = st->vy = 0;
    st_aux->mass = 3;

    st = &_pInitial[1];
    st_aux = &_pAux[1];
    st->x = -2;
    st->y = -1;
    st->vx = st->vy = 0;
    st_aux->mass = 4;

    st = &_pInitial[2];
    st_aux = &_pAux[2];
    st->x = 1;
    st->y = -1;
    st->vx = st->vy = 0;
    st_aux->mass = 5;

    // determine the size of the area including all particles
    for (int i = 0; i < _num; ++i)
    {
        PODState &st = _pInitial[i];
        _max.x = std::max(_max.x, st.x);
        _max.y = std::max(_max.y, st.y);
        _min.x = std::min(_min.x, st.x);
        _min.y = std::min(_min.y, st.y);
    }

    // The Barnes Hut algorithm needs square shaped quadrants.
    // calculate the height of the square including all particles (and a bit more space)
    double l = 1.05 * std::max(_max.x - _min.x,
                               _max.y - _min.y);

    _roi = l * 1.5;

    // compute the center of the region including all particles
    Vec2D c(_min.x + (_max.x - _min.x) / 2.0,
            _min.y + (_max.y - _min.y) / 2.0);
    _min.x = c.x - l / 2.0;
    _max.x = c.x + l / 2.0;
    _min.y = c.y - l / 2.0;
    _max.y = c.y + l / 2.0;

    std::cout << "Initial particle distribution area\n";
    std::cout << "----------------------------------\n";
    std::cout << "Particle spread:\n";
    std::cout << "  xmin=" << _min.x << ", ymin=" << _min.y << "\n";
    std::cout << "  xmax=" << _max.y << ", ymax=" << _max.y << "\n";
    std::cout << "Bounding box:\n";
    std::cout << "  cx =" << c.x << ", cy  =" << c.y << "\n";
    std::cout << "  l  =" << l << "\n";
}


void ModelNBody::CalcBHArea(const ParticleData &data)
{
    /*
      // reset bounding box
      _max.x = _max.y = std::numeric_limits<double>::min();
      _min.x = _min.y = std::numeric_limits<double>::max();

      for (int i=0; i<_num; ++i)
      {
        PODState &s = data._pState[i];

        // determine the size of the area including all particles
        _max.x = std::max(_max.x, s.x);
        _max.y = std::max(_max.y, s.y);
        _min.x = std::min(_min.x, s.x);
        _min.y = std::min(_min.y, s.y);
      }

      // The Barnes Hut algorithm needs square shaped quadrants.
      // calculate the height of the square including all particles (and a bit more space)
      double l = 1.05 * std::max(_max.x - _min.x,
                                 _max.y - _min.y);

      // compute the center of the region including all particles
      Vec2D c(_min.x + (_max.x - _min.x)/2.0,
              _min.y + (_max.y - _min.y)/2.0);
      _min.x = c.x - l/2.0;
      _max.x = c.x + l/2.0;
      _min.y = c.y - l/2.0;
      _max.y = c.y + l/2.0;
    */
}


/** \brief Build the barnes hut tree by adding all particles that are inside
           the region of interest.
*/
void ModelNBody::BuiltTree(const ParticleData &all)
{
    // Reset the quadtree, make sure only particles inside the roi
    // are handled. The renegade ones may live long and prosper
    // outside my simulation
    _root.Reset(Vec2D(_center.x - _roi, _center.y - _roi),
                Vec2D(_center.x + _roi, _center.y + _roi));

    // build the quadtree
    int ct = 0;
    for (int i = 0; i < _num; ++i)
    {
        try
        {
            // extract data for a single particle
            ParticleData p(&(all._pState[i]),
                           &(all._pAuxState[i]));

            // insert the particle, but only if its inside the roi
            _root.Insert(p, 0);
            ++ct;
        }
        catch (std::exception &exc)
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
    _root.ComputeMassDistribution();
    if (_bVerbose)
    {
        std::cout << "Tree Dump\n";
        std::cout << "---------\n";
        _root.DumpNode(-1, 0);
        std::cout << "\n\n";
    }

    // update the center of mass
    _center = _root.GetCenterOfMass();
}


const PODAuxState *ModelNBody::GetAuxState() const
{
    return _pAux;
}


BHTreeNode *ModelNBody::GetRootNode()
{
    return &_root;
}


int ModelNBody::GetN() const
{
    return _num;
}


double ModelNBody::GetTheta() const
{
    return _root.GetTheta();
}


void ModelNBody::SetVerbose(bool bVerbose)
{
    _bVerbose = bVerbose;
}


void ModelNBody::SetTheta(double theta)
{
    _root.SetTheta(theta);
}


void ModelNBody::Eval(double *a_state, double a_time, double *a_deriv)
{
    // wrap the complete particle data together for easier treatment
    // in the following algorithms
    PODState *pState = reinterpret_cast<PODState *>(a_state);
    PODDeriv *pDeriv = reinterpret_cast<PODDeriv *>(a_deriv);
    ParticleData all(pState, _pAux);

    CalcBHArea(all);
    BuiltTree(all);

#pragma omp parallel for
    for (int i = 1; i < _num; ++i)
    {
        ParticleData p(&pState[i], &_pAux[i]);
        Vec2D acc = _root.CalcForce(p);
        pDeriv[i].ax = acc.x;
        pDeriv[i].ay = acc.y;
        pDeriv[i].vx = pState[i].vx;
        pDeriv[i].vy = pState[i].vy;
    }

    // Particle 0 is calculated last, because the statistics
    // data relate to this particle. They would be overwritten
    // otherwise
    _root.StatReset();
    ParticleData p(&pState[0], &_pAux[0]);
    Vec2D acc = _root.CalcForce(p);
    pDeriv[0].ax = acc.x;
    pDeriv[0].ay = acc.y;
    pDeriv[0].vx = pState[0].vx;
    pDeriv[0].vy = pState[0].vy;

    // Save vectors for camera orientations
    //  m_camDir.x = pState[0].x - pState[4000].x;
    //  m_camDir.y = pState[0].y - pState[4000].y;
    _camPos.x = _root.GetCenterOfMass().x;
    _camPos.y = _root.GetCenterOfMass().y;
}


bool ModelNBody::IsFinished(double *state)
{
    return false;
}
