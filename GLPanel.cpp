#include "GLPanel.h"


GLPanel::GLPanel(QWidget *parent) : QGLWidget(parent)
{
	terrain = 0;
	water = 0;
}


GLPanel::~GLPanel()
{

}


void GLPanel::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
	terrain->SetCamera(&cam);
}


void GLPanel::SetWaterLayer(WaterLayer *layer)
{
	water = layer;
	water->SetCamera(&cam);
}


void GLPanel::initializeGL()
{
	glewExperimental = true;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "Error: GLEW unable to load OpenGL Extensions" << std::endl;
		exit(EXIT_FAILURE);
	}

	glClearColor(0.1, 0.2, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
}


void GLPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	cam.SetWindowSize(-1.0*w/h, 1.0*w/h, -1.0, 1.0, -100.0, 100.0);
}


void GLPanel::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (terrain)
		terrain->Draw();
	if (water)
		water->Draw();
}


void GLPanel::wheelEvent(QWheelEvent *event)
{
	cam.Zoom(event->delta());
	updateGL();
}


void GLPanel::mousePressEvent(QMouseEvent *event)
{
	pushedButton = event->button();
	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;
}


void GLPanel::mouseMoveEvent(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	if (pushedButton == Qt::LeftButton)
	{
		cam.Rotate(dx);
		cam.Tilt(dy);
	}
	else if (pushedButton == Qt::RightButton)
	{
		cam.Pan(dx, dy);
	}

	oldx = newx;
	oldy = newy;

	updateGL();
}


void GLPanel::mouseReleaseEvent(QMouseEvent *event)
{

}


void GLPanel::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Right)
	{
		if (water)
		{
			water->LoadTimestep(water->GetCurrentTimestep()+1);
		}
	}
	else if (event->key() == Qt::Key_Left)
	{
		if (water)
		{
			water->LoadTimestep(water->GetCurrentTimestep()-1);
		}
	}
	updateGL();
}
