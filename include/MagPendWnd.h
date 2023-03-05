//--- Standard includes --------------------------------------------------------
#include <vector>

//--- Implementation -----------------------------------------------------------
#include "SDLWnd.h"
#include "IIntegrator.h"
#include "ModelMagPend.h"
#include "Types.h"


//----------------------------------------------------------------------------
class MagPendWnd : public SDLWindow
{
public:

  typedef std::vector<int> result_type;
  typedef std::vector<IIntegrator*> integrator_buf;

  MagPendWnd(int width, int height);

  virtual void Render();
  virtual void PollMessages();
    
  void Init(double x, double y, double vx, double vy);
  const result_type& Calculate(double xstart, double ystart, bool bDumpFile);
  integrator_buf GetIntegratorBuffer();
  const gfx::Color& GetColor(int i) const;

private:
    
  struct Point
  {
    Point(double x, double y, double vx, double vy, double err);
    
    double x, y;
    double vx, vy;
    double err;
  };

  typedef std::vector<Point> trace_type;
  gfx::Color m_aColors[10];

  void Draw();
  void DrawModel();
  void DrawTrace(const trace_type &trace, int color, bool bDumpFile);

  SDL_Event m_event;
  ModelMagPend m_Model;
  integrator_buf m_vIntegrator;
  result_type m_vResults;
  double m_dt;    // Zeitschrittweite
  double m_xTrace;
  double m_yTrace;
};