#ifndef DIALOG_OPEN_H
#define DIALOG_OPEN_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
	class Dialog_Open;
}

class Dialog_Open : public QDialog
{
		Q_OBJECT
		
	public:
		explicit Dialog_Open(QWidget *parent = 0);
		~Dialog_Open();

		QString getFort14();
		QString getFort63();
		
	private slots:
		void on_chooseFort14_clicked();
		void on_chooseFort63_clicked();

	private:
		Ui::Dialog_Open *ui;

		QString fort14;
		QString fort63;
};

#endif // DIALOG_OPEN_H
