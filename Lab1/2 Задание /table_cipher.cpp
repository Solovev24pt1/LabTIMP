#include "table_cipher.h"
#include <vector>
#include <algorithm>
using namespace std;

TableCipher::TableCipher(int key)
{
    if (key <= 0) {
        throw invalid_argument("Ключ должен быть положительным числом");
    }
    columns = key;
}

wstring TableCipher::encrypt(const wstring& plain_text)
{
    if (plain_text.empty()) {
        throw std::invalid_argument("Текст не может быть пустым");
    }
    
    int text_length = plain_text.length();
    int rows = (text_length + columns - 1) / columns;
    vector<vector<wchar_t>> table(rows, vector<wchar_t>(columns, L' '));
    int index = 0;
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (index < text_length) {
                table[row][col] = plain_text[index++];
            } else {
                table[row][col] = L'Я'; 
            }
        }
    }
    wstring result;
    for (int col = columns - 1; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            result += table[row][col];
        }
    }
    
    return result;
}

wstring TableCipher::decrypt(const wstring& cipher_text)
{
    if (cipher_text.empty()) {
        throw invalid_argument("Текст не может быть пустым");
    }
    int total_chars = cipher_text.length();
    int rows = total_chars / columns;
    
    if (total_chars % columns != 0) {
        throw invalid_argument("Некорректная длина зашифрованного текста");
    }
    vector<vector<wchar_t>> table(rows, vector<wchar_t>(columns, L' '));
    int index = 0;
    for (int col = columns - 1; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            table[row][col] = cipher_text[index++];
        }
    }
    wstring result;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            result += table[row][col];
        }
    }
    while (!result.empty() && result.back() == L'Я') {
        result.pop_back();
    }
    return result;
}
