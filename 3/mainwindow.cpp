#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

static std::string red_html = "<font color=\"Red\">";
static std::string black_html = "<font color=\"Black\">";
static std::string end_html = "</font>";

// Конструктор для инициализации программы
MainWindow::MainWindow() : QMainWindow(0), ui(new Ui::MainWindow) {
    ui->setupUi(this);

	coder = new HuffmanCode();
	hamming_coder = new HammingCode();
}

// Деструктор для очистки всех данных после выхода из программы
MainWindow::~MainWindow() {
    delete ui;

	delete coder;
	delete hamming_coder;
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
			std::wstring encoded_text = ui->InitialText_TE->toPlainText().toStdWString();
			std::wstring decoded_text = coder->decodeText(encoded_text);
			ui->ResultText_TE->setText(QString::fromStdWString(decoded_text));

			ui->InitialText_TE->setText(QString::fromStdWString(encoded_text));
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

	ui->Alphabet_LE->setText("");
	ui->Frequency_LE->setText("");
	ui->Codes_LE->setText("");

	ui->WordAverageLength_LBL->setText("0");
	ui->Entropy_LBL->setText("0");
	ui->Redundancy_LBL->setText("0");
	ui->KraftInequality_LBL->setText("нет");
}

void MainWindow::on_HammingEncode_B_clicked() {
	if (hamming_coder->getTable().size() == 0)
		on_MatrixRead_B_clicked();

	hamming_coder->makeCodeHash();

	auto encoded = hamming_coder->encodeTextByTable(ui->InitialText_TE->toPlainText().toStdString());
	ui->ResultText_TE->setText(QString::fromStdString(encoded));
}

void MainWindow::on_MatrixRead_B_clicked() {
	ui->ErrCount_LBL->setText("0");
	ui->ErrsPos_LBL->setText("0");

	if (!ui->MatrixFilename_LE->text().isEmpty() && !ui->MatrixSizesFilename_LE->text().isEmpty() &&
		!ui->AlphabetFilename_LE->text().isEmpty() && !ui->CodesFilename_LE->text().isEmpty()) {
		hamming_coder->clearTable();

		hamming_coder->readAlphabet(ui->AlphabetFilename_LE->text().toStdString(), ui->CodesFilename_LE->text().toStdString());
		auto table = hamming_coder->getTable();

		std::string alpha, codes;
		for (auto sign : table) {
			alpha += std::to_string(sign.sign) + " ";
			codes += sign.code + " ";
		}

		ui->Alphabet_LE->setText(QString::fromStdString(alpha));
		ui->Codes_LE->setText(QString::fromStdString(codes));

		auto matrix = hamming_coder->readMatrixFiles(ui->MatrixFilename_LE->text().toStdString(), ui->MatrixSizesFilename_LE->text().toStdString());
		ui->Matrix_TE->setText(QString::fromStdString(matrix));
	}
	else
		if (ui->AlphabetFilename_LE->text().isEmpty())
			viewError("Не указан файл с алфавитом!");
		else
			if (ui->CodesFilename_LE->text().isEmpty())
				viewError("Не указан файл с кодами!");
			else
				if (ui->MatrixFilename_LE->text().isEmpty())
					viewError("Не указан файл с проверочной матрицей!");
				else
					if (ui->MatrixSizesFilename_LE->text().isEmpty())
						viewError("Не указан файл с размерами матрицы!");
}

void MainWindow::removeSubstrs(std::string& s, std::string& p) {
	std::string::size_type i = s.find(p);
	while (i != std::string::npos) {
		s.erase(i, p.length());
		i = s.find(p, i);
	}
}

void MainWindow::on_HammingDecode_B_clicked() {
	ui->ErrCount_LBL->setText("0");
	ui->ErrsPos_LBL->setText("0");

	if (!ui->Alphabet_LE->text().isEmpty() && !ui->Codes_LE->text().isEmpty() &&
		!ui->Matrix_TE->toPlainText().isEmpty()) {
		auto encoded_text = ui->InitialText_TE->toPlainText().toStdString();

		removeSubstrs(encoded_text, red_html);
		removeSubstrs(encoded_text, black_html);
		removeSubstrs(encoded_text, end_html);

		auto decoded_text = hamming_coder->decodeTextByTable(encoded_text);

		ui->ResultText_TE->setText(QString::fromStdString(decoded_text));
		ui->InitialText_TE->setText(QString::fromStdString(black_html + encoded_text + end_html));

		std::uint8_t err_count = hamming_coder->getErrCount();

		if (err_count) {
			auto errs_pos = hamming_coder->getErrsPos();

			std::string errs_pos_str;
			for (auto err : errs_pos)
				errs_pos_str += std::to_string(err) + ", ";
			errs_pos_str.resize(errs_pos_str.length() - 2);

			for (std::int8_t i = errs_pos.size() - 1; i >= 0; --i) {
				auto err_bit = encoded_text[errs_pos[i] - 1];
				encoded_text = encoded_text.erase(errs_pos[i] - 1, 1);
				encoded_text.insert(errs_pos[i] - 1, red_html + err_bit + end_html);
			}

			ui->InitialText_TE->setText(QString::fromStdString(encoded_text));

			ui->ErrCount_LBL->setText(QString::number(err_count));
			ui->ErrsPos_LBL->setText(QString::fromStdString(errs_pos_str));
		}
	}
	else
		if (ui->AlphabetFilename_LE->text().isEmpty())
			viewError("Не указан файл с алфавитом!");
		else
			if (ui->CodesFilename_LE->text().isEmpty())
				viewError("Не указан файл с кодами!");
			else
				if (ui->MatrixFilename_LE->text().isEmpty())
					viewError("Не указан файл с проверочной матрицей!");
				else
					if (ui->MatrixSizesFilename_LE->text().isEmpty())
						viewError("Не указан файл с размерами матрицы!");
}

void MainWindow::on_HammingClear_B_clicked() {
	hamming_coder->clearTable();

	ui->Alphabet_LE->setText("");
	ui->Codes_LE->setText("");

	ui->Matrix_TE->setText("");

	ui->ErrCount_LBL->setText("0");
	ui->ErrsPos_LBL->setText("0");
}
