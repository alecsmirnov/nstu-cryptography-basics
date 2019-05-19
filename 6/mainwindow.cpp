#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QElapsedTimer>
#include <ctime>

#define NANOSECOND 1e+9

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
}

long long MainWindow::GCD(long long a, long long b) {
	return b == 0 ? a : GCD(b, a % b);
}

long long MainWindow::power(long long a, long long p) {
	if (p == 0)
		return 1;

	if (p % 2 == 0) {
		long long val = power(a, p / 2);
		return val * val;
	}

	return power(a, p - 1);
}

bool MainWindow::fermat(long long n, std::size_t iters) {
	if (n == 2)
		return true;

	srand(std::time(NULL));

	for (std::size_t i = 0; i != iters; ++i) {
		long long a = (rand() % (n - 2)) + 2;

		if (GCD(a, n) != 1)
			return false;

		if (power(a, n - 1) % n != 1)
			return false;
	}

	return true;
}

void MainWindow::setPrimeStatusText(QString text) {
	ui->elapsedTime_LBL->setText("Время: 0.0 (сек) = 0 (нсек)");
	ui->primeStatus_LBL->setText(text);
	ui->primeStatus_LBL->setStyleSheet("color: rgb(0, 0, 0);");
}

void MainWindow::setPrimeStatusTextTrue(QString text) {
	ui->primeStatus_LBL->setText(text);
	ui->primeStatus_LBL->setStyleSheet("color: rgb(0, 170, 0);");
}

void MainWindow::setPrimeStatusTextFalse(QString text) {
	ui->primeStatus_LBL->setText(text);
	ui->primeStatus_LBL->setStyleSheet("color: rgb(255, 0, 0);");
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_pushButton_clicked() {
	auto number = ui->number_TE->toPlainText();
	auto iters_count = ui->itersCount_LE->text();

	if (!number.isEmpty() && !iters_count.isEmpty()) {
		if (1 < number.toLongLong() && 0 < iters_count.toUInt()) {
			QElapsedTimer timer;
			timer.start();
			auto prime = fermat(number.toLongLong(), iters_count.toUInt());
			auto duration = timer.nsecsElapsed();

			double elapsed_time = 1.0 * duration / NANOSECOND;
			ui->elapsedTime_LBL->setText("Время: " + QString::number(elapsed_time, 'f', 9) +
										 " (сек) = " + QString::number(duration) + " (нсек)");

			prime ? setPrimeStatusTextTrue("Число является простым!") :
					setPrimeStatusTextTrue("Число является составным!");
		}
		else
			setPrimeStatusText("Число должно быть больше 1!");
	}
	else
		setPrimeStatusText("Не указано число для проверки");
}
