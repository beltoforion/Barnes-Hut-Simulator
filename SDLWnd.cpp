#include "SDLWnd.h"

//--- Standard includes -------------------------------------------------
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cmath>

#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/glx.h>     // Header file fot the glx libraries.


// static functions / variables
GLuint SDLWindow::s_fontBase = 0;

//-----------------------------------------------------------------------
void SDLWindow::InitFont()
{
  Display *dpy;          /* Our current X display */
  XFontStruct *fontInfo; /* Our font info */

  /* Sotrage for 96 characters */
  s_fontBase = glGenLists(96);

  /* Get our current display long enough to get the fonts */
  dpy = XOpenDisplay(NULL);

  /* Get the font information */
  fontInfo = XLoadQueryFont(dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1" );

  /* If the above font didn't exist try one that should */
  if (fontInfo == NULL)
  {
    fontInfo = XLoadQueryFont(dpy, "fixed");

    /* If that font doesn't exist, something is wrong */
    if (fontInfo == NULL)
        throw std::runtime_error("no X font available?");
  }

  /* generate the list */
  glXUseXFont( fontInfo->fid, 32, 96, s_fontBase);

  /* Recover some memory */
  XFreeFont(dpy, fontInfo);

  /* close the display now that we're done with it */
  XCloseDisplay(dpy);
}

//-----------------------------------------------------------------------
void SDLWindow::KillFont()
{
  glDeleteLists(s_fontBase, 96);
}

//-----------------------------------------------------------------------
/* Print our GL text to the screen */
void SDLWindow::TextOut(const char *fmt, ...)
{
  char text[256]; /* Holds our string */
  va_list ap;     /* Pointer to our list of elements */

  /* If there's no text, do nothing */
  if (fmt == NULL)
    return;

  /* Parses The String For Variables */
  va_start( ap, fmt );

  /* Converts Symbols To Actual Numbers */
  vsprintf( text, fmt, ap );
  va_end(ap);

  glPushAttrib(GL_LIST_BIT);     // Pushes the Display List Bits
  glListBase(s_fontBase - 32);   // Sets base character to 32
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // Draws the text
  glPopAttrib();                 // Pops the Display List Bits
}

//-----------------------------------------------------------------------
void SDLWindow::TextOut(int x, int y, const char *fmt, ...)
{
  Vec3D p = GetOGLPos(x, y);
  glRasterPos2f(p.x, p.y);

  char text[256]; /* Holds our string */
  va_list ap;     /* Pointer to our list of elements */

  /* If there's no text, do nothing */
  if (fmt == NULL)
    return;

  /* Parses The String For Variables */
  va_start( ap, fmt );

  /* Converts Symbols To Actual Numbers */
  vsprintf( text, fmt, ap );
  va_end(ap);

  glPushAttrib(GL_LIST_BIT);     // Pushes the Display List Bits
  glListBase(s_fontBase - 32);   // Sets base character to 32
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // Draws the text
  glPopAttrib();                 // Pops the Display List Bits
}

//------------------------------------------------------------------------------
/** \brief get opengl position from a screen position

   see also:  http://nehe.gamedev.net/data/articles/article.asp?article=13
*/
Vec3D SDLWindow::GetOGLPos(int x, int y)
{
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;

  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );

  winX = (float)x;
  winY = (float)viewport[3] - (float)y;
  glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

  gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

  return Vec3D(posX, posY, posZ);
}

//-----------------------------------------------------------------------
SDLWindow::SDLWindow(int width, int height, double axisLen, const std::string &caption)
  :m_event()
  ,m_fov(axisLen)
  ,m_width(0)
  ,m_height(0)
  ,m_fps(0)
  ,m_idxSnapshot(0)
  ,m_camPos(0, 0, 2)
  ,m_camLookAt(0, 0, 0)
  ,m_camOrient(0, 1, 0)
  ,m_pScreen(NULL)
  ,m_fontBase(0)
  ,m_bRunning(true)
{
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    throw std::runtime_error(SDL_GetError());
  atexit(SDL_Quit);

  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  m_pScreen = SDL_SetVideoMode(width, height, 16, SDL_OPENGL);
  if (!m_pScreen)
    throw std::runtime_error(SDL_GetError());

  SetCaption(caption);

  m_width = width;
  m_height = height;

  InitGL();
}

//-----------------------------------------------------------------------
SDLWindow::~SDLWindow()
{
  KillFont();
  SDL_Quit();
}

//-----------------------------------------------------------------------
void SDLWindow::InitGL()	        // We call this right after our OpenGL window is created.
{
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.1f, 0.0f);  // black background
  glViewport(0, 0, GetWidth(), GetHeight());

  SDLWindow::InitFont();
}

//-----------------------------------------------------------------------
void SDLWindow::SaveToTGA(int idx)
{
  if (idx==-1)
    m_idxSnapshot++;
  else
    m_idxSnapshot = idx;

  std::stringstream ss;
  ss << "frame_" << std::setw(5) << std::setfill('0') << m_idxSnapshot << ".tga";
  SaveToTGA(ss.str());
}

//-----------------------------------------------------------------------
void SDLWindow::SaveToTGA(const std::string &sName)
{
  using std::ios;

  int nSize = GetWidth() * GetHeight() * 3;

  GLubyte pixels[nSize];
  glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels);

  std::string sFile;
  if (sName.length())
    sFile = sName;
  else
  {
    // use default name with time stamp
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    if (tmp==NULL)
      sFile = "snapshot.tga";
    else
    {
      char szTime[1024];
      if (strftime(szTime, sizeof(szTime), "snapshot_%Y%m%d_%H%M%S.tga", tmp) == 0)
        sFile = "snapshot.tga";
      else
        sFile = szTime;
    }
  }

  std::fstream file(sFile.c_str(), ios::out|ios::binary|ios::trunc);
  char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
  char header[6] = { (char)(GetWidth()  % 256),
                     (char)(GetWidth()  / 256),
                     (char)(GetHeight() % 256),
                     (char)(GetHeight() / 256),
                     24,
                     0 };
  file.write(TGAheader, sizeof(TGAheader));
  file.write(header, sizeof(header));

  //convert to BGR format
  for (int i=0; i<nSize; i+=3)
    std::swap(pixels[i], pixels[i+2]);

  file.write(reinterpret_cast<char*>(pixels), nSize);
  file.close();
}

//------------------------------------------------------------------------------
void SDLWindow::ScaleAxis(double scale)
{
  m_fov *= scale;
  AdjustCamera();
}

//------------------------------------------------------------------------------
const Vec3D& SDLWindow::GetCamPos() const
{
  return m_camPos;
}

//------------------------------------------------------------------------------
const Vec3D& SDLWindow::GetCamOrient() const
{
  return m_camOrient;
}

//------------------------------------------------------------------------------
const Vec3D& SDLWindow::GetCamLookAt() const
{
  return m_camLookAt;
}

//------------------------------------------------------------------------------
void SDLWindow::SetCameraOrientation(const Vec3D &orient)
{
  m_camOrient = orient;
  AdjustCamera();
}

//------------------------------------------------------------------------------
void SDLWindow::SetCamera(const Vec3D &pos, const Vec3D &lookAt, const Vec3D &orient)
{
  m_camOrient = orient;
  m_camPos = pos;
  m_camLookAt = lookAt;
  AdjustCamera();
}

//------------------------------------------------------------------------------
void SDLWindow::AdjustCamera()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  double l = m_fov/2.0;
  glOrtho(-l, l, -l, l, -l, l);
  gluLookAt(m_camPos.x, m_camPos.y, m_camPos.z,
            m_camLookAt.x, m_camLookAt.y, m_camLookAt.z,
            m_camOrient.x, m_camOrient.y, m_camOrient.z);
  glMatrixMode(GL_MODELVIEW);
}

//-----------------------------------------------------------------------
void SDLWindow::SetCaption(const std::string &caption)
{
  SDL_WM_SetCaption(caption.c_str(), NULL);
}

//-----------------------------------------------------------------------
double SDLWindow::GetFOV() const
{
  return m_fov;
}

//-----------------------------------------------------------------------
int SDLWindow::GetFPS() const
{
  return m_fps;
}

//------------------------------------------------------------------------------
void SDLWindow::DrawAxis(const Vec2D &origin)
{
  glColor3f(0.3, 0.3, 0.3);

  double s = std::pow(10, (int)(log10(m_fov/2))),
         l = m_fov/100,
         p = 0;

  glPushMatrix();
  glTranslated(origin.x, origin.y, 0);

  for (int i=0; p<m_fov; ++i)
  {
    p += s;

    if (i%2==0)
    {
      glRasterPos2f(p-l, -4*l);
      TextOut("%2.0f", p);
    }
    else
    {
      glRasterPos2f(p-l, 2*l);
      TextOut("%2.0f", p);
    }

    glBegin(GL_LINES);
      glVertex3f(p, -l, 0);
      glVertex3f(p,  l, 0);

      glVertex3f(-p, -l, 0);
      glVertex3f(-p,  0, 0);
      glVertex3f(-l, p, 0);
      glVertex3f( 0, p, 0);
      glVertex3f(-l, -p, 0);
      glVertex3f( 0, -p, 0);
    glEnd();

  }

  glBegin(GL_LINES);
    glVertex3f(-m_fov, 0, 0);
    glVertex3f(m_fov, 0, 0);
    glVertex3f(0, -m_fov, 0);
    glVertex3f(0, m_fov, 0);
  glEnd();

  glPopMatrix();
}

//-----------------------------------------------------------------------
void SDLWindow::MainLoop()
{
  int ct = 0;
  double dt = 0;
  time_t t1(time(NULL)), t2;

  while (m_bRunning)
  {
    Render();
    PollEvents();
    ++ct;

    t2 = time(NULL);
    dt = difftime(t2, t1);
    if (dt>1)
    {
      m_fps = (double)ct / dt;
      ct = 0;
      t1 = t2;
    }
  }
}

//-----------------------------------------------------------------------
SDL_Surface *SDLWindow::Surface()
{
  return m_pScreen;
}

//-----------------------------------------------------------------------
int SDLWindow::GetWidth() const
{
  return m_width;
}

//-----------------------------------------------------------------------
int SDLWindow::GetHeight() const
{
  return m_height;
}

//-----------------------------------------------------------------------
void SDLWindow::ExitMainLoop()
{
  m_bRunning = false;
}

//------------------------------------------------------------------------------
void SDLWindow::OnProcessEvents(uint8_t type)
{}

//------------------------------------------------------------------------------
void SDLWindow::PollEvents()
{
  while (SDL_PollEvent(&m_event))
  {
    switch (m_event.type)
    {
    case SDL_QUIT:
        ExitMainLoop();
        break;

    default:
        OnProcessEvents(m_event.type);
        break;
    } // switch event type
  }
}

