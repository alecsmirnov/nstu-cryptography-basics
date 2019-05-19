#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	long long GCD(long long a, long long b);
	long long power(long long a, long long p);
	bool fermat(long long n, std::size_t iters);

	void setPrimeStatusText(QString text);
	void setPrimeStatusTextTrue(QString text);
	void setPrimeStatusTextFalse(QString text);

private:
	Ui::MainWindow* ui;

private slots:
	void on_pushButton_clicked();
};

#endif
