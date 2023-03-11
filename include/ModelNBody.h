#ifndef _MODEL_N_BODY_H
#define	_MODEL_N_BODY_H

//---------------------------------------------------------------------
#include "IModel.h"
#include "Vector.h"
#include "BHTree.h"
#include "Types.h"



/** \brief Model class for handling th n-body problem. */
class ModelNBody : public IModel
{
public:

    ModelNBody();
    virtual ~ModelNBody();

    void Init();
    void InitCollision();
    void Init3Body();

    virtual void Eval(double *state, double time, double *deriv);
    virtual bool IsFinished(double *state);
    virtual double* GetInitialState();

    double GetSuggestedTimeStep() const;
    double GetTimeUnit() const;
    double GetTheta() const;
    BHTreeNode* GetRootNode();
    const PODAuxState* GetAuxState() const;
    int GetN() const;

    const Vec3D& GetCamDir() const;
    const Vec3D& GetCamPos() const;
    Vec3D GetCenterOfMass() const;

    void SetTheta(double theta);
    void SetVerbose(bool bVerbose);
    void SetROI(double roi);
    double GetROI() const;

private:

    void CalcBHArea(const ParticleData &p);
    void BuiltTree(const ParticleData &p);
    void GetOrbitalVelocity(const ParticleData &p1, const ParticleData &p2);
    void ResetDim(int num, double stepsize);

    PODState *_pInitial;        ///< The initial state
    PODAuxState *_pAux;         ///< Auxilliary state information

    BHTreeNode _root;           ///< The root node of the barnes hut tree
    Vec2D _min;                 ///< Upper left corner of the bounding box containing all particles
    Vec2D _max;                 ///< Lower right corner of the bounding box containing all particles
    Vec2D _center;              ///< The center of the simulation, the barnes hut tree is centered at this point
    Vec3D _camDir;              ///< Direction of the camera
    Vec3D _camPos;              ///< Position of the camera
    double _roi;
    double _timeStep;

    const double mass_sun;
    const double pc_in_m;
    const double gamma_si;
    const double gamma_1;
    const double time_1;
    
    int _num;
    bool _bVerbose;
};

#endif

