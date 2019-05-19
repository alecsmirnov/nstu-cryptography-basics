#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

// Конструктор для инициализации программы
MainWindow::MainWindow() : QMainWindow(0), ui(new Ui::MainWindow) {
    ui->setupUi(this);

	coder = new HuffmanCode();
}

// Деструктор для очистки всех данных после выхода из программы
MainWindow::~MainWindow() {
    delete ui;

	delete coder;
}

// Установка информации о кодируемом алфавите
void MainWindow::setLabels() const {
	// Вывод информации
	if (!coder->getTable().empty()) {
		// Средняя длина слова
		ui->WordAverageLength_LBL->setText(QString::number(coder->getWordAverageLength()));
		// Энтропия
		ui->Entropy_LBL->setText(QString::number(coder->getEntropy()));
		// Избыточность
		ui->Redundancy_LBL->setText(QString::number(coder->getRedundancy()));
		// Выполнение неравенства Крафта
		coder->kraftInequality() ? ui->KraftInequality_LBL->setText("да") :
								   ui->KraftInequality_LBL->setText("нет");
	}
}

// Утсановка кодируемого алфавита, вероятностей и кодов
void MainWindow::setAlphabet() const {
	auto table = coder->getTable();

	// Формирование строк с последовательным перечнем
	// символов алфавита, их вероятностей и кодовых слов
	std::wstring alphabet, frequency, codes;
	for (auto elem : table) {
		alphabet += wcharToWstring(elem.sign) + L" ";
		frequency += std::to_wstring(elem.frequency) + L" ";
		codes += elem.code + L" ";
	}

	// Вывод сформированной информации
	ui->Alphabet_LE->setText(QString::fromStdWString(alphabet));
	ui->Frequency_LE->setText(QString::fromStdWString(frequency));
	ui->Codes_LE->setText(QString::fromStdWString(codes));
}

// Вывести предупреждающее сообщение
void MainWindow::viewInfo(std::string info_text) const {
	QMessageBox::information(nullptr, QObject::tr("Информация"), tr(info_text.c_str()));
}

// Вывести сообщение об ошибке
void MainWindow::viewError(std::string err_text) const {
	QMessageBox::critical(nullptr, QObject::tr("Ошибка"), tr(err_text.c_str()));
}

// Преобразовать спец-символ в текстовый символ.
// например: символ новой строки в '\n' или провебла в ' '
std::wstring MainWindow::wcharToWstring(wchar_t sign) const {
	std::wstring result;
	if (sign == L'\n')
		result = L"\'\\n\'";
	else
		if (sign == L' ')
			result = L"\' \'";
		else
			result = sign;

	return result;
}

// Поведение для кнопки кодирования текста
void MainWindow::on_Encode_B_clicked() {
	if (!ui->InitialText_TE->toPlainText().isEmpty()) {
		coder->clearTable();

		// Два случая кодирования текста:
		// 1) Кодирование текста по уже сформированной таблице Хаффмана.
		// 2) Формирование таблицы Хаффмана по подсчёту вероятностей в исходном текста,
		//    с последующим кодирование текста
		std::wstring encoded_text;
		if (!ui->Alphabet_LE->text().isEmpty() && !ui->Frequency_LE->text().isEmpty()) {
			// Формируем таблицу по введённому алфавиту и вероятностям
			coder->encodeAlphabet(ui->Alphabet_LE->text().toStdWString(),
								  ui->Frequency_LE->text().toStdWString());

			// Кодируем текст по уже существующей таблице Хаффмана
			encoded_text = coder->encodeTextByTable(ui->InitialText_TE->toPlainText().toStdWString());
			ui->ResultText_TE->setText(QString::fromStdWString(encoded_text));
		}
		else {
			// Формируем таблицу Хаффмана по заданному тексту и кодируем его
			encoded_text = coder->encodeText(ui->InitialText_TE->toPlainText().toStdWString());
			ui->ResultText_TE->setText(QString::fromStdWString(encoded_text));
		}

		setAlphabet();
		setLabels();
	}
	else
		viewError("Поле с исходным текстом не может быть пустым!");
}

// Поведение для кнопки декодирования текста
void MainWindow::on_Decode_B_clicked() {
	if (!ui->InitialText_TE->toPlainText().isEmpty()) {
		// Декодируем текст по уже существующей таблице Хаффмана
		if (!coder->getTable().empty()) {
			std::wstring decoded_text = coder->decodeText(ui->InitialText_TE->toPlainText().toStdWString());
			ui->ResultText_TE->setText(QString::fromStdWString(decoded_text));
		}
		else
			viewError("Таблица кодирования и декодирования не может быть пустой!");
	}
	else
		viewError("Поле с исходным текстом не может быть пустым!");
}

// Поведение для кнопки чтения входного текста из файла
void MainWindow::on_InitialTextRead_B_clicked() {
	if (!ui->InitialTextFilename_LE->text().isEmpty()) {
		std::string text = coder->readFile(ui->InitialTextFilename_LE->text().toStdString());

		if (!text.empty())
			ui->InitialText_TE->setText(QString::fromStdString(text));
		else
			viewError("Файл " + ui->InitialTextFilename_LE->text().toStdString() + " не существует!");
	}
}

// Поведение для кнопки записи результирующего текста в файл
void MainWindow::on_ResultTextWrite_B_clicked() {
	if (!ui->ResultTextFilename_LE->text().isEmpty()) {
		QFile file(ui->ResultTextFilename_LE->text());

		// Запись файла в его полном представлении со всем спец-символами
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			  QTextStream out(&file);
			  out << ui->ResultText_TE->toPlainText();

			  viewInfo("Результат записан в:\n" + ui->ResultTextFilename_LE->text().toStdString());
		}
	}
}

// Поведение для кнопки чтения алфавита из файла
void MainWindow::on_AlphabetRead_B_clicked() {
	if (!ui->AlphabetFilename_LE->text().isEmpty()) {
		std::string text = coder->readFile(ui->AlphabetFilename_LE->text().toStdString());

		if (!text.empty())
			ui->Alphabet_LE->setText(QString::fromStdString(text));
		else
			viewError("Файл " + ui->AlphabetFilename_LE->text().toStdString() + " не существует!");
	}
}

// Поведение для кнопки чтения вероятностей из файла
void MainWindow::on_FrequencyRead_B_clicked() {
	if (!ui->FrequencyFilename_LE->text().isEmpty()) {
		std::string text = coder->readFile(ui->FrequencyFilename_LE->text().toStdString());

		if (!text.empty())
			ui->Frequency_LE->setText(QString::fromStdString(text));
		else
			viewError("Файл " + ui->FrequencyFilename_LE->text().toStdString() + " не существует!");
	}
}

// Поведение для кнопки формирования кодовых слов по алфавиту и вероятностям
void MainWindow::on_CodesCreate_B_clicked() {
	if (!ui->Alphabet_LE->text().isEmpty() && !ui->Frequency_LE->text().isEmpty()) {
		coder->clearTable();
		// Формируем таблицу по введённому алфавиту и вероятностям
		coder->encodeAlphabet(ui->Alphabet_LE->text().toStdWString(), ui->Frequency_LE->text().toStdWString());

		setAlphabet();
		setLabels();
	}
	else
		viewError("Поля с алфавитом и вероятностями не могут быть пустыми!");
}

// Поведение для кнопки записи алфавита в файл
void MainWindow::on_AlphabetWrite_B_clicked() {
	if (!ui->Alphabet_LE->text().isEmpty()) {
		coder->writeFile(ui->AlphabetFilename_LE->text().toStdString(), ui->Alphabet_LE->text().toStdWString());

		viewInfo("Результат записан в:\n" + ui->AlphabetFilename_LE->text().toStdString());
	}
}

// Поведение для кнопки записи вероятностей в файл
void MainWindow::on_FrequencyWrite_B_clicked() {
	if (!ui->Frequency_LE->text().isEmpty()) {
		coder->writeFile(ui->FrequencyFilename_LE->text().toStdString(), ui->Frequency_LE->text().toStdWString());

		viewInfo("Результат записан в:\n" + ui->FrequencyFilename_LE->text().toStdString());
	}
}

// Поведение для кнопки записи кодовых слов в файл
void MainWindow::on_CodesWrite_B_clicked() {
	if (!ui->Codes_LE->text().isEmpty()) {
		coder->writeFile(ui->CodesFilename_LE->text().toStdString(), ui->Codes_LE->text().toStdWString());

		viewInfo("Результат записан в:\n" + ui->CodesFilename_LE->text().toStdString());
	}
}

// Поведение для кнопки очистки кодовой таблицы и дерева
void MainWindow::on_Clear_B_clicked() {
	coder->clearTable();

	//ui->InitialTextFilename_LE->setText("");
	//ui->InitialText_TE->setText("");

	//ui->ResultTextFilename_LE->setText("");
	//ui->ResultText_TE->setText("");

	//ui->AlphabetFilename_LE->setText("");
	ui->Alphabet_LE->setText("");

	//ui->FrequencyFilename_LE->setText("");
	ui->Frequency_LE->setText("");

	//ui->CodesFilename_LE->setText("");
	ui->Codes_LE->setText("");

	ui->WordAverageLength_LBL->setText("0");
	ui->Entropy_LBL->setText("0");
	ui->Redundancy_LBL->setText("0");
	ui->KraftInequality_LBL->setText("нет");
}
