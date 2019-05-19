#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LCGenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	Ui::MainWindow* ui;
	LCGenerator generator;

	QString vecToQString(std::vector<std::size_t> V);

private slots:
	void on_addVec_PB_clicked();
	void on_clearTW_PB_clicked();
	void on_comboBox_currentIndexChanged(int index);
	void on_createSequence_PB_clicked();
};

#endif
