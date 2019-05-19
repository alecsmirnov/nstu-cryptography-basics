#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "HuffmanCode.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
	explicit MainWindow();

    ~MainWindow();

private slots:
	// Поведение для кнопки кодирования текста
	void on_Encode_B_clicked();
	// Поведение для кнопки декодирования текста
	void on_Decode_B_clicked();

	// Поведение для кнопки чтения входного текста из файла
	void on_InitialTextRead_B_clicked();
	// Поведение для кнопки записи результирующего текста в файл
	void on_ResultTextWrite_B_clicked();

	// Поведение для кнопки чтения алфавита из файла
	void on_AlphabetRead_B_clicked();
	// Поведение для кнопки чтения вероятностей из файла
	void on_FrequencyRead_B_clicked();
	// Поведение для кнопки формирования кодовых слов по алфавиту и вероятностям
	void on_CodesCreate_B_clicked();

	// Поведение для кнопки записи алфавита в файл
	void on_AlphabetWrite_B_clicked();
	// Поведение для кнопки записи вероятностей в файл
	void on_FrequencyWrite_B_clicked();
	// Поведение для кнопки записи кодовых слов в файл
	void on_CodesWrite_B_clicked();

	// Поведение для кнопки очистки кодовой таблицы и дерева
	void on_Clear_B_clicked();

private:
	// Установка информации о кодируемом алфавите
	void setLabels() const;
	// Утсановка кодируемого алфавита, вероятностей и кодов
	void setAlphabet() const;
	// Вывести предупреждающее сообщение
	void viewInfo(std::string info_text) const;
	// Вывести сообщение об ошибке
	void viewError(std::string err_text) const;
	// Преобразовать спец-символ в текстовый символ.
	// например: символ новой строки в '\n' или провебла в ' '
	std::wstring wcharToWstring(wchar_t sign) const;

private:
	Ui::MainWindow* ui;

	HuffmanCode* coder;
};

#endif
