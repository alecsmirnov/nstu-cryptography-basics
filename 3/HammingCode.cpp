#include "HammingCode.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace HammingStd;

void HammingCode::readAlphabet(std::string alphabet, std::string codes) {
    std::ifstream alpha(alphabet);	// Строки в виде потока
    std::ifstream code(codes);		// Для обработки подобно файлу

    if (alpha && code) {
		std::pair<int, std::string> sign;
		while (alpha >> sign.first && code >> sign.second)
			code_table.push_back({sign.first, sign.second, std::string()});
	}
}

std::string HammingCode::readMatrixFiles(std::string matrix, std::string sizes) {
    std::ifstream matrix_file(matrix);
    std::ifstream sizes_file(sizes);

    std::string str_file;

    if (sizes_file && matrix_file) {
		int in_n, in_k;
		sizes_file >> in_n >> in_k;

		n = in_n;
		r = in_n - in_k;

		H = new char*[r];
		for (std::uint8_t i = 0; i != r; ++i)
			H[i] = new char[n];

		for (std::uint8_t i = 0; i != r; ++i)
			for (std::uint8_t j = 0; j != n; ++j)
				matrix_file >> H[i][j];

		for (std::uint8_t i = 0; i != r; ++i) {
			for (std::uint8_t j = 0; j != n; ++j) {
				str_file += H[i][j];

				if (j != n - 1)
					str_file += " ";
			}

			str_file += "\n";
		}

		str_file.resize(str_file.length() - 1);

		matrix_file.close();
		sizes_file.close();
    }
    else
		std::cerr << "Error: " + matrix + " or " + sizes + " is not opened!" << std::endl;

    return str_file;
}

void HammingCode::makeCodeHash() {
    if (H && !code_table.empty()) {
		std::vector<std::pair<std::uint8_t, std::uint8_t>> bits;

		for (std::uint8_t j = 0; j != n; ++j) {
			std::uint8_t sum = 0;
			std::uint8_t curr_i = 0;
			for (std::uint8_t i = 0; i != r; ++i)
				if (H[i][j] == '1') {
					++sum;
					curr_i = i;
				}

			if (sum == 1)
				bits.push_back(std::make_pair(curr_i, j));
		}

		for (auto &sign : code_table) {
			sign.code_hash = std::string(n, '*');

			for (std::uint8_t i = 0; i != bits.size(); ++i)
				sign.code_hash[bits[i].second] = ' ';

			for (std::uint8_t code_idx = 0, i = 0; i != n; ++i)
				if (sign.code_hash[i] != ' ')
					sign.code_hash[i] = sign.code[code_idx++];

			for (std::uint8_t i = 0; i != bits.size(); ++i) {
				std::string hash;
				for (std::uint8_t j = 0; j != n; ++j)
					if (H[bits[i].first][j] == '1' && j != bits[i].second)
						hash += sign.code_hash[j];

				char bit = chrToInt(hash[0]);
				for (std::uint8_t j = 1; j != hash.size(); ++j)
					bit = (bit ^ chrToInt(hash[j]));

				sign.code_hash[bits[i].second] = intToChr(bit);
			}
		}
    }
}

std::string HammingCode::encodeTextByTable(std::string text) {
    std::string encoded_text;

    for (auto chr : text) {
		bool find = true;
		for (auto it = code_table.begin(); it != code_table.end() && find; ++it)
			if (intToChr(it->sign) == chr) {
				encoded_text += it->code_hash;
				find = false;
			}
    }

    return encoded_text;
}

std::string HammingCode::decodeTextByTable(std::string text) {
    std::string decoded_text;
    std::vector<std::size_t>().swap(errs_pos);

    for (std::size_t chr_i = 0; chr_i < text.size(); chr_i += n) {
		std::string syndrome;
		for (std::uint8_t i = 0; i != r; ++i) {
			std::string code;
			for (std::uint8_t j = 0; j != n; ++j)
				code += intToChr(chrToInt(text[chr_i + j]) * chrToInt(H[i][j]));

			char bit = chrToInt(code[0]);
			for (std::uint8_t j = 1; j != code.size(); ++j)
				bit = (bit ^ chrToInt(code[j]));

			syndrome += intToChr(bit);
		}

		std::string code;
		for (std::uint8_t i = 0; i != n; ++i)
			code += text[chr_i + i];

		std::uint8_t syndrome_size = std::strtoull(syndrome.c_str(), nullptr, 2);

		if (syndrome_size) {
			code[syndrome_size - 1] = code[syndrome_size - 1] == '1' ? '0' : '1';

			errs_pos.push_back(chr_i + syndrome_size);
		}

		bool find = true;
		for (auto it = code_table.begin(); it != code_table.end() && find; ++it)
			if (it->code_hash == code) {
				find = false;
				decoded_text += std::to_string(it->sign);
			}
    }

    return decoded_text;
}

std::vector<HammingStd::elem_t> HammingCode::getTable() const {
    return code_table;
}

std::uint8_t HammingCode::getErrCount() const {
    return errs_pos.size();
}

std::vector<std::size_t> HammingCode::getErrsPos() const {
    return errs_pos;
}

void HammingCode::clearTable() {
    std::vector<elem_t>().swap(code_table);

    r = n = 0;
    for (std::uint8_t i = 0; i != r; ++i)
		delete[] H[i];
    delete[] H;
    H = nullptr;

    std::vector<std::size_t>().swap(errs_pos);
}
