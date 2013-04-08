#ifndef GLPANEL_H
#define GLPANEL_H

#include "glew.h"
#include "GLCamera.h"
#include "TerrainLayer.h"
#include "WaterLayer.h"
#include <QGLWidget>
#include <iostream>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>

class GLPanel : public QGLWidget
{
		Q_OBJECT
	public:
		explicit	GLPanel(QWidget *parent = 0);
		~GLPanel();

		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetWaterLayer(WaterLayer *layer);

	protected:

		Camera		cam;
		TerrainLayer	*terrain;
		WaterLayer	*water;

		void	initializeGL();
		void	resizeGL(int w, int h);
		void	paintGL();

		void	wheelEvent(QWheelEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);
		void	mouseReleaseEvent(QMouseEvent *event);
		void	keyReleaseEvent(QKeyEvent *event);


	private:

		int	oldx, oldy, newx, newy, dx, dy;
		bool	mouseMoved;
		enum	Qt::MouseButton pushedButton;
};


#endif
