#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->centralWidget->layout()->setContentsMargins(0,0,0,0);

	terrain = 0;
	water = 0;
}

MainWindow::~MainWindow()
{
	delete ui;

	if (terrain)
		delete terrain;
	if (water)
		delete water;
}

void MainWindow::on_actionOpen_triggered()
{
	Dialog_Open dlg;
	dlg.setWindowTitle(tr("Open sweSolver Results"));
	if (dlg.exec() == QDialog::Accepted && !dlg.getFort14().isEmpty())
	{
		terrain = new TerrainLayer();
		terrain->SetFort14(dlg.getFort14().toStdString());
		ui->glPanel->SetTerrainLayer(terrain);

		if (!dlg.getFort63().isEmpty())
		{
			water = new WaterLayer();
			water->SetTerrainLayer(terrain);
			water->SetFort63(dlg.getFort63().toStdString());
			ui->glPanel->SetWaterLayer(water);
		}
	}
}
