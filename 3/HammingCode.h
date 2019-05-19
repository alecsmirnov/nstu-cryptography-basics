#ifndef HAMMINGCODE_H
#define HAMMINGCODE_H

#include <string>
#include <vector>

namespace HammingStd {		// Элемент таблицы Хэмминга
struct elem_t {
    int sign;				// Символ алфавита
    std::string code;		// Кодовое слово
    std::string code_hash;
};
}

// Класс для работы с алгоритмом Хэмминга
class HammingCode {
public:
    void readAlphabet(std::string alphabet, std::string codes);
    std::string readMatrixFiles(std::string matrix, std::string sizes);
    void makeCodeHash();

    std::string encodeTextByTable(std::string text);
    std::string decodeTextByTable(std::string text);

    std::vector<HammingStd::elem_t> getTable() const;
    std::uint8_t getErrCount() const;
    std::vector<std::size_t> getErrsPos() const;

    void clearTable();

private:
    std::uint8_t n = 0;
    std::uint8_t r = 0;

    char** H = nullptr;

    std::vector<HammingStd::elem_t> code_table;

    std::vector<std::size_t> errs_pos;
};

#endif
