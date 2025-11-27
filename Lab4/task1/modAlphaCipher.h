/**
 * @file modAlphaCipher.h
 * @author Соловьев Арсений Евгеньевич
 * @version 1.0
 * @date 2025
 * @copyright ПГУ
 * @brief Заголовочный файл для модуля шифрования методом Гронсфельда
 * @warning Реализация для русского языка
 */

#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>
using namespace std;

/**
 * @brief Класс-исключение для ошибок шифрования
 * @details Наследуется от std::invalid_argument
 */
class cipher_error: public invalid_argument {
public:
    /**
     * @brief Конструктор с параметром string
     * @param what_arg сообщение об ошибке
     */
    explicit cipher_error (const string& what_arg):
        invalid_argument(what_arg) {}
    
    /**
     * @brief Конструктор с параметром const char*
     * @param what_arg сообщение об ошибке
     */
    explicit cipher_error (const char* what_arg):
        invalid_argument(what_arg) {}
};

/**
 * @brief Класс для шифрования методом Гронсфельда
 * @details Реализует шифрование и расшифрование текста на русском языке
 */
class modAlphaCipher
{
private:
    wstring alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< Русский алфавит
    map<wchar_t, int> alphaIndex; ///< Ассоциативный массив "символ-номер"
    vector<int> keySeq; ///< Числовая последовательность ключа
    
    /**
     * @brief Преобразование строки в числовой вектор
     * @param s входная строка
     * @return вектор чисел
     */
    vector<int> toNums(const wstring& s);
    
    /**
     * @brief Преобразование числового вектора в строку
     * @param v входной вектор
     * @return строка
     */
    wstring toStr(const vector<int>& v);
    
    /**
     * @brief Валидация и нормализация ключа
     * @param s исходный ключ
     * @return валидированный ключ
     * @throw cipher_error если ключ пустой, содержит недопустимые символы или вырожден
     */
    wstring getValidKey(const wstring& s);
    
    /**
     * @brief Валидация и нормализация открытого текста
     * @param s исходный открытый текст
     * @return валидированный открытый текст
     * @throw cipher_error если текст пустой после удаления не-букв
     */
    wstring getValidOpenText(const wstring& s);
    
    /**
     * @brief Валидация зашифрованного текста
     * @param s исходный зашифрованный текст
     * @return валидированный зашифрованный текст
     * @throw cipher_error если текст пустой или содержит недопустимые символы
     */
    wstring getValidCipherText(const wstring& s);
    
public:
    /**
     * @brief Удаленный конструктор по умолчанию
     */
    modAlphaCipher() = delete;
    
    /**
     * @brief Конструктор с установкой ключа
     * @param keyStr строковый ключ
     * @throw cipher_error если ключ невалиден
     */
    modAlphaCipher(const wstring& keyStr);
    
    /**
     * @brief Шифрование открытого текста
     * @param plain открытый текст
     * @return зашифрованный текст
     * @throw cipher_error если текст невалиден
     */
    wstring encrypt(const wstring& plain);
    
    /**
     * @brief Расшифрование зашифрованного текста
     * @param cipher зашифрованный текст
     * @return расшифрованный текст
     * @throw cipher_error если текст невалиден
     */
    wstring decrypt(const wstring& cipher);
};
