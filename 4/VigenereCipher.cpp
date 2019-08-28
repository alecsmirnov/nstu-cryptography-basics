#include "VigenereCipher.h"

static constexpr char SPACE = ' ';			// Разделяющий символ в тексте

// Задать алфавит сообщения
void VigenereCipher::setAlphabet(Alphabet alphabet) {
	this->alphabet = alphabet;

	// Вычислить основание алфавита по его типу
	switch (alphabet) {
		case Alphabet::NUMBERS:		  alphabet_pow = 10; break;
		case Alphabet::UPPERCASE_LAT:
		case Alphabet::LOWERCASE_LAT: alphabet_pow = 26; break;
		default: break;
	}
}

// Задать список ключевых слов
void VigenereCipher::setKeywords(std::vector<std::string> keywords) {
	this->keywords = keywords;
}

// Получить текущий список ключевых слов
std::vector<std::string> VigenereCipher::getKeywords() const {
	return keywords;
}

// Очистить список ключевых слов
void VigenereCipher::clearKeywords() {
	std::vector<std::string>().swap(keywords);
}

// Зашифровать текст по заданному алфавиту и ключевым словам
std::string VigenereCipher::startEncrypt(std::string text) {
	std::string encrypted_text = text;
	clearSpaces();
	rememberSpaces(text);

	// Последовательное (один или более раз) шифрование текста по
	// указанным ключевым словам
	for (auto it = keywords.begin(); it != keywords.end(); ++it)
		encrypted_text = encrypt(encrypted_text, getKey(encrypted_text, *it));

	return restoreSpaces(encrypted_text);
}

// Расшифровать текст по заданному алфавиту и ключевым словам
std::string VigenereCipher::startDecrypt(std::string text) {
	std::string decrypted_text = text;

	// Последовательное (один или более раз) расшрование текста по
	// указанным ключевым словам (в обратном порядке)
	for (auto it = keywords.rbegin(); it != keywords.rend(); ++it)
		decrypted_text = decrypt(decrypted_text, getKey(decrypted_text, *it));

	return restoreSpaces(decrypted_text);
}

// Сформировать ключ по заданному тексту и ключевому слову
std::string VigenereCipher::getKey(std::string text, std::string keyword) const {
	for (std::string::size_type i = 0; keyword.length() != text.length(); ++i)
		keyword.push_back(keyword[i %= text.length()]);

	return keyword;
}

// Зашифровать текст по ключу
std::string VigenereCipher::encrypt(std::string text, std::string key) const {
	std::string encrypted_text;

	// Сложение двух символов по модулю мощности алфавита и получение зашифованного
	for (std::string::size_type i = 0; i != text.length(); ++i) {
		char new_char = (charToInt(text[i]) + charToInt(key[i])) % alphabet_pow;

		encrypted_text.push_back(intToChar(new_char));
	}

	return encrypted_text;
}

// Расшифровать текст по ключу
std::string VigenereCipher::decrypt(std::string encrypted_text, std::string key) const {
	std::string decrypted_text;

	for (std::string::size_type i = 0; i != encrypted_text.length(); ++i) {
		char origin_char = (charToInt(encrypted_text[i]) - charToInt(key[i]) + alphabet_pow) % alphabet_pow;

		decrypted_text.push_back(intToChar(origin_char));
	}

	return decrypted_text;
}

// Преобразовать символ в число
std::uint8_t VigenereCipher::charToInt(char x) const {
	return x - static_cast<char>(alphabet);
}

// Преобразовать число в символ
char VigenereCipher::intToChar(std::uint8_t x) const {
	return x + static_cast<char>(alphabet);
}

// Запомнить позиции разделяющих символов
void VigenereCipher::rememberSpaces(std::string text) {
	for (std::string::size_type i = 0; i != text.length(); ++i)
		if (text[i] == SPACE)
			spaces.push_back(i);
}

// Восстановить разделяющие символы в тексте
std::string VigenereCipher::restoreSpaces(std::string text) {
	for (auto space_pos : spaces)
		text[space_pos] = SPACE;

	return text;
}

// Очистить список позиций разделяющих символов
void VigenereCipher::clearSpaces() {
	std::vector<std::string::size_type>().swap(spaces);
}
