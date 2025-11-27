/**
 * @file test.cpp
 * @author Соловьев Арсений Евгеньевич
 * @version 1.0
 * @date 2025
 * @brief Модульные тесты для класса modAlphaCipher
 * @details Тестирование функциональности шифрования и обработки ошибок
 */

#include <UnitTest++/UnitTest++.h>
#include <string>
#include "modAlphaCipher.h"
using namespace std;

/**
 * @brief Преобразование широкой строки в UTF-8
 * @param ws широкая строка
 * @return строка в UTF-8
 */
 
string wideToUtf8(const wstring& ws) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

/**
 * @brief Преобразование строки UTF-8 в широкую строку
 * @param s строка в UTF-8
 * @return широкая строка
 */
 
wstring utf8ToWide(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

/// Макрос для сравнения широких строк в тестах
#define CHECK_WIDE_EQUAL(expected, actual) \
    CHECK_EQUAL(wideToUtf8(expected), wideToUtf8(actual))

/**
 * @brief Тестовый набор для конструктора
 * @details Проверяет различные сценарии инициализации ключа
 */
 
SUITE(ConstructorTest)
{
    TEST(ValidKey) {
        modAlphaCipher cipher(L"БВГ");
        CHECK_WIDE_EQUAL(L"БВГБВ", cipher.encrypt(L"ААААА"));
    }
    
    TEST(LongKey) {
        modAlphaCipher cipher(L"БВГДЕЁЖЗИЙК");
        CHECK_WIDE_EQUAL(L"БВГДЕ", cipher.encrypt(L"ААААА"));
    }
    
    TEST(LowCaseKey) {
        modAlphaCipher cipher(L"бвг");
        CHECK_WIDE_EQUAL(L"БВГБВ", cipher.encrypt(L"ААААА"));
    }
    
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б1"), cipher_error);
    }
    
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б,В"), cipher_error);
    }
    
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б В"), cipher_error);
    }
    
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }
    
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cipher(L"ААА"), cipher_error);
    }
}

/**
 * @brief Фикстура для тестов с ключом "Б"
 * @details Создает экземпляр шифратора с ключом "Б" для повторного использования
 */
 
struct KeyBFixture {
    modAlphaCipher* cipher;
    
    KeyBFixture() {
        cipher = new modAlphaCipher(L"Б");
    }
    
    ~KeyBFixture() {
        delete cipher;
    }
};

/**
 * @brief Тестовый набор для метода encrypt
 * @details Проверяет различные сценарии шифрования
 */
 
SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyBFixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"РСЙГЁУНЙС", cipher->encrypt(L"ПРИВЕТМИР"));
    }
    
    TEST_FIXTURE(KeyBFixture, LowerCaseString) {
        CHECK_WIDE_EQUAL(L"РСЙГЁУНЙС", cipher->encrypt(L"приветмир"));
    }
    
    TEST_FIXTURE(KeyBFixture, StringWithWhitespaceAndPunctuation) {
        CHECK_WIDE_EQUAL(L"РСЙГЁУНЙС", cipher->encrypt(L"ПРИВЕТ, МИР!"));
    }
    
    TEST_FIXTURE(KeyBFixture, StringWithNumbers) {
        CHECK_WIDE_EQUAL(L"ТОПГЬНДПЕПН", cipher->encrypt(L"С Новым 2024 Годом"));
    }
    
    TEST_FIXTURE(KeyBFixture, EmptyString) {
        CHECK_THROW(cipher->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, NoAlphaString) {
        CHECK_THROW(cipher->encrypt(L"12314"), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        CHECK_WIDE_EQUAL(L"ОПЗБДСЛЗП", cipher.encrypt(L"ПРИВЕТМИР"));
    }
}

/**
 * @brief Тестовый набор для метода decrypt
 * @details Проверяет различные сценарии расшифрования
 */
 
SUITE(DecryptTest)
{
    TEST_FIXTURE(KeyBFixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"ПРИВЕТМИР", cipher->decrypt(L"РСЙГЁУНЙС"));
    }
    
    TEST_FIXTURE(KeyBFixture, LowerCaseString) {
        CHECK_THROW(cipher->decrypt(L"рсйгёуНЙС"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, WhitespaceString) {
        CHECK_THROW(cipher->decrypt(L"РСЙ ГЁУ НЙС"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, DigitsString) {
        CHECK_THROW(cipher->decrypt(L"ТПГРДН2019ЕФДПЕ"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, PunctuationString) {
        CHECK_THROW(cipher->decrypt(L"РСЙ,ГЖУ!НИТ"), cipher_error);
    }
    
    TEST_FIXTURE(KeyBFixture, EmptyString) {
        CHECK_THROW(cipher->decrypt(L""), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher(L"Я");
        CHECK_WIDE_EQUAL(L"ПРИВЕТМИР", cipher.decrypt(L"ОПЗБДСЛЗП"));
    }
}

/**
 * @brief Главная функция тестов
 * @param argc количество аргументов
 * @param argv массив аргументов
 * @return результат выполнения тестов
 */
 
int main(int argc, char** argv)
{
    return UnitTest::RunAllTests();
}
