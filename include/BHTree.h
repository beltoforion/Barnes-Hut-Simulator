#ifndef BH_TREE_H
#define BH_TREE_H

//--- Standard includes --------------------------------------------------------
#include <vector>

//--- Implementation -----------------------------------------------------------
#include "Vector.h"
#include "Types.h"


//------------------------------------------------------------------------------
struct State
{
    Vec2D pos;
    Vec2D vel;
    Vec2D acc;
    double m;
};



/** \brief Implementation of a single node of the barnes hut tree. */
class BHTreeNode
{
public:
    /** \brief Enumeration for the quadrants. */
    enum EQuadrant
    {
        NE = 0,
        NW,
        SW,
        SE,
        NONE
    };

    /** \brief A structure for holding debug information. */
    struct DebugStat
    {
        int _nNumCalc; ///< Total number of calculations for estimating the force
    };

    BHTreeNode(const Vec2D &min,
               const Vec2D &max,
               BHTreeNode *parent = nullptr);
    ~BHTreeNode();

    void Reset(const Vec2D &min,
               const Vec2D &max);

    bool IsRoot() const;
    bool IsExternal() const;
    bool WasTooClose() const;

    void StatReset();
    int StatGetNumCalc() const;

    int GetNumRenegades() const;
    int GetNum() const;
    const Vec2D &GetCenterOfMass() const;
    const Vec2D &GetMin() const;
    const Vec2D &GetMax() const;

    double GetTheta() const;
    void SetTheta(double theta);

    void Insert(const ParticleData &newParticle, int level);

    EQuadrant GetQuadrant(double x, double y) const;
    BHTreeNode *CreateQuadNode(EQuadrant eQuad);

    void ComputeMassDistribution();

    Vec2D CalcForce(const ParticleData &p) const;
    void DumpNode(int quad, int level);

public:
    BHTreeNode *_quadNode[4];

private:
    Vec2D CalcAcc(const ParticleData &p1, const ParticleData &p2) const;
    Vec2D CalcTreeForce(const ParticleData &p) const;

    /** \brief Data for the particle.

      Only valid if this is a leaf node.
    */
    ParticleData _particle;

    double _mass;              ///< Mass of all particles inside the node
    Vec2D _cm;                 ///< Center of Mass
    Vec2D _min;                ///< Upper left edge of the node
    Vec2D _max;                ///< Lower right edge of the node
    Vec2D _center;             ///< Center of the node
    BHTreeNode *_parent;       ///< The parent node
    int _num;                  ///< The number of particles in this node
    mutable bool _bSubdivided; ///< True if this node is too close to use the approximation for the force calculation

    static double s_theta;
    static std::vector<ParticleData> s_renegades;

public:
    static double s_gamma;

private:
    static double s_soft;
    static DebugStat s_stat;
};

#endif // BH_TREE_H
