#ifndef _SDL_WINDOW_H
#define _SDL_WINDOW_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_opengl.h> // opengl support
#include <GL/gl.h>          // Header File For The OpenGL32 Library
#include <GL/glu.h>         // Header File For The GLu32 Library

#include "Vector.h"


/** \brief Basic infrastructure for grafical output using SDL/OpenGL */
class SDLWindow
{
public:
    SDLWindow(int width, int height, double axisLen, const std::string &caption);
    virtual ~SDLWindow();
    void MainLoop();
    void ExitMainLoop();
    void SetCaption(const std::string &caption);
    int GetWidth() const;
    int GetHeight() const;
    virtual void Render() = 0;
    virtual void Update() = 0;

protected:
    virtual void PollEvents();
    virtual void OnProcessEvents(uint8_t type);

    //-----------------------------------------
    // Camera setup
    //-----------------------------------------

    const Vec3D &GetCamPos() const;
    const Vec3D &GetCamOrient() const;
    const Vec3D &GetCamLookAt() const;
    void SetCameraOrientation(const Vec3D &orientation);
    void SetCamera(const Vec3D &pos, const Vec3D &lookAt, const Vec3D &orient);
    void AdjustCamera();

    //-----------------------------------------
    // Basic graphics functionality
    //-----------------------------------------

    void DrawAxis(const Vec2D &origin);
    int GetFPS() const;
    void SaveToTGA(const std::string &sName);
    void SaveToTGA(int idx = -1);

    //-----------------------------------------
    // misc
    //-----------------------------------------

    void ScaleAxis(double scale);
    double GetFOV() const;
    SDL_Surface *Surface();
    SDL_Event _event;

    static void InitFont();
    static void KillFont();
    static void TextOut(const char *fmt, ...);
    static void TextOut(int x, int y, const char *fmt, ...);
    static Vec3D GetOGLPos(int x, int y);

    static GLuint s_fontBase;

private:
    void InitGL();

    double _fov; ///< Length of an axis
    int _width;  ///< Width of the window in pixel
    int _height; ///< Height of the window in pixel
    int _fps;
    int _idxSnapshot;

    Vec3D _camPos;    ///< Position of the camera
    Vec3D _camLookAt; ///< Point atwhich the camera is aimed
    Vec3D _camOrient; ///< orientation of the camera (rotation as it aims at its target)

    SDL_Surface *_pScreen;

    volatile bool _bRunning;
};

#endif