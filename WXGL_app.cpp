#include "WXGL_app.h"

Texture *texture = nullptr;

IMPLEMENT_APP(GLApp);

bool GLApp::OnInit()
{
	GLFrame* frame = new GLFrame(NULL,
		_T("OpenGL App"),
		wxDefaultPosition,
		wxSize(windowWidth, windowHeight));
	frame->Show(true);
	return true;
}

GLFrame::GLFrame(wxWindow* parent,
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size,
	long style)
	: wxFrame(parent, wxID_ANY, title, pos, size, style)
{
	canvas = new GLCanvas(this);
}

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_IDLE				(GLCanvas::OnIdle)
	EVT_PAINT				(GLCanvas::OnPaint)
	EVT_ERASE_BACKGROUND	(GLCanvas::OnEraseBackground)
	EVT_KEY_UP				(GLCanvas::OnKeyUp)
	EVT_KEY_DOWN			(GLCanvas::OnKeyDown)
	EVT_TIMER				(wxID_ANY, GLCanvas::OnTimer)
END_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
	: wxGLCanvas(parent, (wxGLCanvas*)NULL, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name)
{
	cubeRotateX = 45.0;
	cubeRotateY = 45.0f;
	initialized = false;

	for (int i = 0; i < 500; i++)
		keys[i] = false;

	timer = new wxTimer(this, wxID_ANY);
	timer->Start(1);
}

GLCanvas::~GLCanvas()
{
	if (timer != nullptr)
	{
		delete timer;
		timer = nullptr;
	}
}

// OpenGL functoins
GLvoid GLCanvas::establishProjectionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 200.0);
}

GLvoid GLCanvas::initGL(GLsizei width, GLsizei height)
{
	establishProjectionMatrix(width, height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
	glEnable(GL_TEXTURE_2D);

	texture = new Texture("Assets/Textures/surface.bmp");
}

GLvoid GLCanvas::displayFPS(GLvoid)
{
	static wxUint32 lastTime = wxGetLocalTimeMillis().GetLo();
	static int loops = 0;
	static float fps = 0.0f;
	wxUint32 newTime = wxGetLocalTimeMillis().GetLo();

	if (newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime) * 1000.0;
		fps = (fps + newFPS) / 2.0;

		char title[80];
		sprintf(title, "OpenGL Demo - %f", fps);

		// todo
		// display fps as text

		lastTime = newTime;
		loops = 0;
	}
	loops++;
}

GLvoid GLCanvas::drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(cubeRotateX, 1.0, 0.0, 0.0);
	glRotatef(cubeRotateY, 0.0, 1.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture->texID);
	// draw cube
	glBegin(GL_QUADS);
		//glColor3f(1.0, 0.5, 0.0); // top orange face
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,  1.0,  1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0,  1.0);

		//glColor3f(0.0, 0.0, 1.0); // bottom blue face
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);

		//glColor3f(1.0, 1.0, 0.0); // right yellow face
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f( 1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f( 1.0, -1.0,  1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0,  1.0);

		//glColor3f(0.0, 1.0, 0.0); // left green face
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0,  1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0,  1.0,  1.0);

		//glColor3f(1.0, 0.0, 0.0); // front red face
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0, -1.0,  1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0,  1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,  1.0,  1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0,  1.0);

		//glColor3f(1.0, 0.0, 1.0); // back purple face
		glTexCoord2f(1.0, 1.0); glVertex3f( 1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0,  1.0, -1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f( 1.0,  1.0, -1.0);
	glEnd();

	glFlush();
	SwapBuffers();
}

GLboolean GLCanvas::checkKeys(GLvoid)
{
	if (keys[WXK_UP] == true)
		cubeRotateX -= 5.0;
	if (keys[WXK_DOWN] == true)
		cubeRotateX += 5.0;
	if (keys[WXK_LEFT] == true)
		cubeRotateY -= 5.0;
	if (keys[WXK_RIGHT] == true)
		cubeRotateY += 5.0;

	return keys[WXK_ESCAPE];
}


// WX Event Handlers
void GLCanvas::OnIdle(wxIdleEvent& event)
{
	Refresh();
}

void GLCanvas::OnPaint(wxPaintEvent& event)
{
	if (checkKeys())
	{
		delete texture;
		GetParent()->Close(true);
	}

	wxPaintDC dc(this);

	SetCurrent();

	if (!initialized)
	{
		initGL(GetSize().GetWidth(), GetSize().GetHeight());
		initialized = true;
	}

	drawScene();
}

void GLCanvas::OnEraseBackground(wxEraseEvent& event)
{
	// this function does nothing but, stops wxwidgets from causing stuttering
}

void GLCanvas::OnKeyDown(wxKeyEvent& event)
{
	int key = event.GetKeyCode();

	keys[key] = true;

	event.Skip();
}

void GLCanvas::OnKeyUp(wxKeyEvent& event)
{
	int key = event.GetKeyCode();

	keys[key] = false;

	event.Skip();
}

void GLCanvas::OnTimer(wxTimerEvent& event)
{
	Refresh();
}
