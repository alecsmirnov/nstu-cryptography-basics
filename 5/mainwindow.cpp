#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	ui->K_LE->setText("1");
	ui->length_LE->setText("10");

	ui->tableWidget->setColumnCount(2);
	ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("a"));
	ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("b"));

	ui->tableWidget->setColumnWidth(0, (ui->tableWidget->width() - 21.01) / 2);
	ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width() - 21.01) / 2);

	ui->alpha_LBL->setText(QChar(0xb1, 0x03) + QString(" = 0.05"));
	ui->pearsonTest_LBL->hide();
}

QString MainWindow::vecToQString(std::vector<std::size_t> V) {
	QString str;

	for (auto elem : V)
		str += QString::number(elem) + " ";
	str.resize(str.size() - 1);

	return str;
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_addVec_PB_clicked() {
	auto a = ui->a_LE->text();
	auto b = ui->b_LE->text();

	if (!a.isEmpty() && !b.isEmpty()) {
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(a));
		ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(b));

		ui->a_LE->setText("");
		ui->b_LE->setText("");

		ui->tableWidget->setColumnWidth(0, (ui->tableWidget->width() - 41.01) / 2);
		ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width() - 41.01) / 2);
	}
}

void MainWindow::on_clearTW_PB_clicked() {
	ui->tableWidget->setColumnWidth(0, (ui->tableWidget->width() - 21.01) / 2);
	ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width() - 21.01) / 2);

	ui->tableWidget->setRowCount(0);
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
	ui->r_LBL->setText(QString::number(index + 1));
}

void MainWindow::on_createSequence_PB_clicked() {
	auto N = ui->N_LE->text();
	auto K = ui->K_LE->text();
	auto length = ui->length_LE->text();

	if (!N.isEmpty() && !K.isEmpty() && !length.isEmpty() && ui->tableWidget->rowCount()) {
		generator.clearB();

		generator.setAlphabetPow(N.toUInt());
		generator.setInitialValue(K.toUInt());

		for (auto i = 0; i != ui->tableWidget->rowCount(); ++i)
			generator.addLCGVector({ui->tableWidget->item(i, 0)->text().toUInt(),
									ui->tableWidget->item(i, 1)->text().toUInt()});

		auto sequence = generator.getSequence(length.toUInt());

		ui->result_TE->setText(vecToQString(sequence));
		ui->period_LBL->setText(QString::number(generator.getPeriodLength(length.toUInt())));

		auto r = ui->comboBox->currentIndex() + 1;
		auto S = generator.getPearsonChiSquared(sequence, r + 1);
		auto Skp = generator.getQuantile(r);

		ui->S_LBL->setText(QString::number(S));
		ui->Skp_LBL->setText(QString::number(Skp));

		auto pearson_test = generator.testPearsonHypothesis(S, Skp);

		QString text = pearson_test ? "S* < Skp => Верно!" : "S* > Skp => Неверно!";
		ui->pearsonTest_LBL->setText(text);
		ui->pearsonTest_LBL->show();
	}
}
