#ifndef _NBODYWND_H
#define _NBODYWND_H

#include <stdint.h>
#include <fstream>

#include "SDLWnd.h"
#include "BHTree.h"
#include "ModelNBody.h"
#include "IIntegrator.h"


/** \brief Main window of th n-body simulation. */
class NBodyWnd final : public SDLWindow
{
public:

    NBodyWnd(int sz, std::string caption);
    virtual ~NBodyWnd();

    virtual void Render() override;
    virtual void Update() override;
    virtual void OnProcessEvents(uint8_t type) override;

    void Init(int num);

private:

    enum DisplayState : unsigned int
    {
        dspNONE = 0,
        dspAXIS = 1 << 0,
        dspBODIES = 1 << 1,
        dspSTAT = 1 << 2,
        dspTREE = 1 << 3,
        dspTREE_COMPLETE = 1 << 4,
        dspCENTER_OF_MASS = 1 << 5,
        dspPAUSE = 1 << 6,
        dspVERBOSE = 1 << 7,
        dspHELP = 1 << 8,
        dspARROWS = 1 << 9,
        dspROI = 1 << 10
    };

    NBodyWnd(const NBodyWnd &orig);

    void DrawBodies();
    void DrawStat();
    void DrawTree();
    void DrawHelp();
    void DrawROI();
    void DrawCenterOfMass();
    void DrawNode(BHTreeNode *pNode, int level);

    ModelNBody *_pModel;
    IIntegrator *_pSolver;

    int _camOrient;  ///< Index of the camera orientation to use
    uint32_t _flags; ///< The display flags
    bool _bDumpState = false;
    bool _bDumpImage = false;
    std::ofstream _outfile;
};

#endif
