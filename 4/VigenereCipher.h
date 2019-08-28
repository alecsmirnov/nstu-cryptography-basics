#ifndef VIGENERECIPHER_H
#define VIGENERECIPHER_H

#include <string>
#include <vector>
#include <cstdint>

enum class Alphabet : char {	// Тип алфавита сообщения
	NUMBERS		  = '0',
	UPPERCASE_LAT = 'A',
	LOWERCASE_LAT = 'a'
};

class VigenereCipher {
public:
	// Задать алфавит сообщения
	void setAlphabet(Alphabet alphabet);

	// Задать список ключевых слов
	void setKeywords(std::vector<std::string> keywords);
	// Получить текущий список ключевых слов
	std::vector<std::string> getKeywords() const;
	// Очистить список ключевых слов
	void clearKeywords();

	// Зашифровать текст по заданному алфавиту и ключевым словам
	std::string startEncrypt(std::string text);
	// Расшифровать текст по заданному алфавиту и ключевым словам
	std::string startDecrypt(std::string text);

private:
	// Сформировать ключ по заданному тексту и ключевому слову
	std::string getKey(std::string text, std::string keyword) const;

	// Зашифровать текст по ключу
	std::string encrypt(std::string text, std::string key) const;
	// Расшифровать текст по ключу
	std::string decrypt(std::string encrypted_text, std::string key) const;

	// Преобразовать символ в число
	char chrToInt(char x);
	// Преобразовать число в символ
	char intToChr(char x);

	// Запомнить позиции разделяющих символов
	void rememberSpaces(std::string text);
	// Восстановить разделяющие символы в тексте
	std::string restoreSpaces(std::string text);
	// Очистить список позиций разделяющих символов
	void clearSpaces();

private:
	Alphabet alphabet;
	std::uint8_t alphabet_pow;

	std::vector<std::string> keywords;
	std::vector<std::string::size_type> spaces;
};

inline char VigenereCipher::chrToInt(char x) {
	return x - '0';
}

inline char VigenereCipher::intToChr(char x) {
	return x + '0';
}

#endif
