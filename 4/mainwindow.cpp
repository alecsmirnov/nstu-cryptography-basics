#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	coder = new VigenereCipher();
}

MainWindow::~MainWindow() {
	delete ui;
	delete coder;
}

// Прочитать текст из файла
std::string MainWindow::readFile(std::string filename) const {
	std::ifstream file(filename);
	std::string str_file;

	if (file) {
		std::string line;
		while(std::getline(file, line))
			str_file += line + "\n";

		str_file.resize(str_file.length() - 1);
		file.close();
	}
	else
		viewError("\"" + filename + "\" is not opened!");

	return str_file;
}

// Записать текст в файл
void MainWindow::writeFile(std::string filename, std::string text) const {
	std::ofstream file(filename, std::ios_base::trunc);
	file << text;
	file.close();
}

// Получить набор кодовых слов из строки кодовых слов
std::vector<std::string> MainWindow::getKeywordsSet(std::string keywords_text) const {
	std::vector<std::string> keywords;

	std::stringstream ss_text(keywords_text);

	std::string keyword;
	while(ss_text >> keyword)
		keywords.push_back(keyword);

	return keywords;
}

// Определить алфавит по символу текста
Alphabet MainWindow::getAlphabetByChar(QChar x) const {
	Alphabet alphabet;

	if (x.isDigit())
		alphabet = Alphabet::NUMBERS;
	else
		if (x.isUpper())
			alphabet = Alphabet::UPPERCASE_LAT;
		else
			if (x.isLower())
				alphabet = Alphabet::LOWERCASE_LAT;

	return alphabet;
}

// Вывести сообщение с информацией
void MainWindow::viewInfo(std::string info_text) const {
	QMessageBox::information(nullptr, QObject::tr("Информация"), tr(info_text.c_str()));
}

// Вывести сообщение с ошибкой
void MainWindow::viewError(std::string err_text) const {
	QMessageBox::critical(nullptr, QObject::tr("Ошибка"), tr(err_text.c_str()));
}

// Прочитать текст из файла
void MainWindow::on_readSourceText_B_clicked() {
	if (!ui->sourceTextFilename_LE->text().isEmpty()) {
		auto source_text = readFile(ui->sourceTextFilename_LE->text().toStdString());
		ui->sourceText_TE->setText(QString::fromStdString(source_text));
	}
	else
		viewError("Название файла с текстом не может быть пустым!");
}

// Прочитать ключевые слова из файла
void MainWindow::on_readKeywords_B_clicked() {
	if (!ui->keywordsFilename_LE->text().isEmpty()) {
		auto keywords_text = readFile(ui->keywordsFilename_LE->text().toStdString());
		ui->keywords_LE->setText(QString::fromStdString(keywords_text));
	}
	else
		viewError("Название файла с ключевыми словами не может быть пустым!");
}

// Записать результат в файл
void MainWindow::on_writeResultText_B_clicked() {
	if (!ui->resultTextFilename_LE->text().isEmpty()) {
		if (!ui->resultText_TE->toPlainText().isEmpty()) {
			auto filename = ui->resultTextFilename_LE->text().toStdString();
			auto result_text = ui->resultText_TE->toPlainText().toStdString();
			writeFile(filename, result_text);
			viewInfo("Текст успешно записан в файл \"" + filename + "\"!");
		}
		else
			viewError("Нельзя записать в файл пустой текст!");
	}
	else
		viewError("Название файла с текстом не может быть пустым!");
}

// Зашифровать текст по ключевым словам
void MainWindow::on_encrypt_B_clicked() {
	auto text = ui->sourceText_TE->toPlainText();
	auto keywords_text = ui->keywords_LE->text();

	if (!text.isEmpty() && !keywords_text.isEmpty()) {
		coder->clearKeywords();

		coder->setAlphabet(getAlphabetByChar(text[0]));
		coder->setKeywords(getKeywordsSet(keywords_text.toStdString()));

		auto encrypted_text = coder->startEncrypt(text.toStdString());
		ui->resultText_TE->setText(QString::fromStdString(encrypted_text));
	}
	else
		viewError("Поле с текстом и ключевыми словами не может быть пустым!");
}

// Расшифровать текст по ключевым словам
void MainWindow::on_decrypt_B_clicked() {
	auto text = ui->sourceText_TE->toPlainText();
	auto keywords_text = ui->keywords_LE->text();

	if (!text.isEmpty() && !keywords_text.isEmpty()) {
		coder->clearKeywords();

		coder->setAlphabet(getAlphabetByChar(text[0]));
		coder->setKeywords(getKeywordsSet(keywords_text.toStdString()));

		auto decrypted_text = coder->startDecrypt(text.toStdString());
		ui->resultText_TE->setText(QString::fromStdString(decrypted_text));
	}
	else
		viewError("Поле с текстом и ключевыми словами не может быть пустым!");
}
