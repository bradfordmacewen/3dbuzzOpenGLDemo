#pragma once

#include <wx/wx.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <wx/glcanvas.h>

#include "Texture.h"

const GLsizei windowWidth = 480;
const GLsizei windowHeight = 360;

class GLApp : public wxApp
{
public:
	bool OnInit();
};

class GLCanvas;

class GLFrame : public wxFrame
{
public:
	GLFrame(wxWindow* parent, 
		const wxString& title, 
		const wxPoint& pos, 
		const wxSize& size, 
		long style = wxDEFAULT_FRAME_STYLE);
private:
	GLCanvas* canvas;
};

class GLCanvas : public wxGLCanvas
{
public:
	GLCanvas(wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = _T("GLCanvas"));
	~GLCanvas();

	// OpenGL functoins
	GLvoid establishProjectionMatrix(GLsizei width, GLsizei height);
	GLvoid initGL(GLsizei width, GLsizei height);
	GLvoid displayFPS(GLvoid);
	GLvoid drawScene(GLvoid);
	GLboolean checkKeys(GLvoid);

	// WX Event Handlers
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnTimer(wxTimerEvent& event);
private:
	bool keys[500];
	float cubeRotateX;
	float cubeRotateY;
	bool initialized;
	wxTimer* timer;

	DECLARE_EVENT_TABLE()
};
