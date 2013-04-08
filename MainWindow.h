#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Dialog_Open.h"
#include "TerrainLayer.h"
#include "WaterLayer.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
	private slots:
		void on_actionOpen_triggered();

	private:
		Ui::MainWindow *ui;

		TerrainLayer	*terrain;
		WaterLayer	*water;
};

#endif // MAINWINDOW_H
