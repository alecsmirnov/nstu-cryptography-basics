#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "VigenereCipher.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	// Прочитать текст из файла
	std::string readFile(std::string filename) const;
	// Записать текст в файл
	void writeFile(std::string filename, std::string text) const;

	// Получить набор кодовых слов из строки кодовых слов
	std::vector<std::string> getKeywordsSet(std::string keywords_text) const;

	// Определить алфавит по символу текста и ключевого слова
	Alphabet getAlphabetByChar(QChar x) const;

	// Вывести сообщение с информацией
	void viewInfo(std::string info_text) const;
	// Вывести сообщение с ошибкой
	void viewError(std::string err_text) const;

private:
	Ui::MainWindow *ui;
	VigenereCipher* coder;

private slots:
	// Прочитать текст из файла
	void on_readSourceText_B_clicked();
	// Прочитать ключевые слова из файла
	void on_readKeywords_B_clicked();

	// Записать результат в файл
	void on_writeResultText_B_clicked();

	// Зашифровать текст по ключевым словам
	void on_encrypt_B_clicked();
	// Расшифровать текст по ключевым словам
	void on_decrypt_B_clicked();
};

#endif
