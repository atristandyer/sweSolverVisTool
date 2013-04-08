#include "Dialog_Open.h"
#include "ui_Dialog_Open.h"

Dialog_Open::Dialog_Open(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_Open)
{
	ui->setupUi(this);

	fort14 = "";
	fort63 = "";
}

Dialog_Open::~Dialog_Open()
{
	delete ui;
}

QString Dialog_Open::getFort14()
{
	return fort14;
}

QString Dialog_Open::getFort63()
{
	return fort63;
}

void Dialog_Open::on_chooseFort14_clicked()
{
	fort14 = QFileDialog::getOpenFileName(this, tr("Choose fort.14 File"));
	ui->fort14Location->setText(fort14);
}

void Dialog_Open::on_chooseFort63_clicked()
{
	fort63 = QFileDialog::getOpenFileName(this, tr("Choose fort.63 File"));
	ui->fort63Location->setText(fort63);
}
