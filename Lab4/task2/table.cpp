/**
 * @file table.cpp
 * @author Соловьев Арсений Евгеньевич
 * @version 1.0
 * @date 2025
 * @brief Реализация методов класса Table для табличной маршрутной перестановки
 */

#include "table.h"
#include <algorithm>
#include <vector>
#include <cwctype>  
using namespace std;

/**
 * @brief Валидация ключа (количества столбцов)
 * @param key исходный ключ
 * @return валидированный ключ
 * @throw cipher_error если ключ неположительный
 */
 
int Table::getValidKey(const int key)
{
    if (key <= 0)
        throw cipher_error("Invalid key: key must be positive");
    return key;
}

/**
 * @brief Валидация и нормализация открытого текста
 * @param s исходный открытый текст
 * @return валидированный текст в верхнем регистре без пробелов и не-букв
 * @throw cipher_error если текст пустой после обработки
 */
 
wstring Table::getValidOpenText(const wstring& s)
{
    wstring tmp;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (auto c : s) {
        if (!iswspace(c)) { 
            if (upper.find(c) != wstring::npos) {
                tmp.push_back(c);
            } else {
                size_t pos = lower.find(c);
                if (pos != wstring::npos) {
                    tmp.push_back(upper[pos]); 
                }
            }
        }
    }
    if (tmp.empty())
        throw cipher_error("Empty open text");
    return tmp;
}

/**
 * @brief Валидация зашифрованного текста
 * @param s исходный зашифрованный текст
 * @return валидированный зашифрованный текст
 * @throw cipher_error если текст пустой или содержит недопустимые символы
 */
 
wstring Table::getValidCipherText(const wstring& s)
{
    if (s.empty())
        throw cipher_error("Empty cipher text");
    
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Whitespace in cipher text");
        }
    }
    
    wstring tmp;
    for (auto c : s) {
        tmp.push_back(c);
    }
    
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";  
    for (auto c : tmp) {
        if (upper.find(c) == wstring::npos)
            throw cipher_error("Invalid cipher text");
    }
    return tmp;
}

/**
 * @brief Конструктор класса Table
 * @param key количество столбцов таблицы
 * @throw cipher_error если ключ невалиден
 */
 
Table::Table(int key)
{
    cols = getValidKey(key);
}

/**
 * @brief Шифрование открытого текста табличной перестановкой
 * @param plain открытый текст для шифрования
 * @return зашифрованный текст
 * @details Маршрут записи: по горизонтали слева направо, сверху вниз
 * @details Маршрут считывания: сверху вниз, справа налево
 * @throw cipher_error если открытый текст невалиден
 */
 
wstring Table::encrypt(const wstring& plain)
{
    wstring validText = getValidOpenText(plain);
    int n = static_cast<int>(validText.length());
    int rows = (n + cols - 1) / cols;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    // Заполнение таблицы по горизонтали слева направо, сверху вниз
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (pos < n) {
                grid[r][c] = validText[pos++];
            }
        }
    }

    wstring out;
    out.reserve(n);
    
    // Считывание таблицы сверху вниз, справа налево
    for (int c = cols - 1; c >= 0; --c) {
        for (int r = 0; r < rows; ++r) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}

/**
 * @brief Расшифрование зашифрованного текста табличной перестановкой
 * @param cipher зашифрованный текст для расшифрования
 * @return расшифрованный текст
 * @details Обратный процесс шифрованию с учетом маршрутов
 * @throw cipher_error если зашифрованный текст невалиден
 */
 
wstring Table::decrypt(const wstring& cipher)
{
    wstring validText = getValidCipherText(cipher);
    int n = static_cast<int>(validText.length());
    int rows = (n + cols - 1) / cols;
    
    int fullCols = n % cols;
    if (fullCols == 0) fullCols = cols;

    vector<vector<wchar_t>> grid(rows, vector<wchar_t>(cols, L' '));
    int pos = 0;

    // Заполнение таблицы по маршруту считывания (сверху вниз, справа налево)
    for (int c = cols - 1; c >= 0; --c) {
        int h = rows;
    
        if (c >= fullCols) {
            h = rows - 1;
        }
        for (int r = 0; r < h; ++r) {
            if (pos < n) {
                grid[r][c] = validText[pos++];
            }
        }
    }

    wstring out;
    out.reserve(n);
    
    // Считывание таблицы по маршруту записи (по горизонтали слева направо, сверху вниз)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] != L' ') {
                out += grid[r][c];
            }
        }
    }
    return out;
}
