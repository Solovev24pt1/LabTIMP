#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <stdexcept>
#include "table_cipher.h"
using namespace std;

wstring stringToWstring(const string& str)
{
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    } catch (const exception& e) {
        throw runtime_error("Ошибка конвертации строки: " + string(e.what()));
    }
}

string wstringToString(const wstring& wstr)
{
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    } catch (const exception& e) {
        throw runtime_error("Ошибка конвертации строки: " + string(e.what()));
    }
}
bool isValidRussianText(const wstring& text)
{
    wstring validChars = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    for (wchar_t c : text) {
        if (validChars.find(c) == wstring::npos) {
            return false;
        }
    }
    return !text.empty();
}

wstring toUpperRussian(const wstring& text)
{
    static const wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    static const wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    wstring result = text;
    for (wchar_t& c : result) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos) {
            c = upper[pos];
        }
    }
    return result;
}

wstring cleanText(const wstring& text)
{
    wstring result;
    wstring validChars = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    for (wchar_t c : text) {
        if (validChars.find(c) != wstring::npos) {
            result += c;
        }
    }
    return result;
}

int inputInteger(const string& prompt)
{
    string input;
    cout << prompt;
    getline(cin, input);
    
    try {
        int value = stoi(input);
        if (value <= 0) {
            throw out_of_range("Число должно быть положительным");
        }
        return value;
    } catch (const invalid_argument&) {
        throw invalid_argument("Введите корректное число");
    } catch (const out_of_range&) {
        throw out_of_range("Число слишком большое");
    }
}

wstring inputText(const string& prompt)
{
    string input;
    cout << prompt;
    getline(cin, input);
    
    if (input.empty()) {
        throw invalid_argument("Текст не может быть пустым");
    }
    
    wstring text = stringToWstring(input);
    text = toUpperRussian(text);
    text = cleanText(text);
    
    if (text.empty()) {
        throw invalid_argument("Текст должен содержать русские буквы");
    }
    
    return text;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "=== Шифр табличной перестановки ===" << endl;
    
    try {
        int key = inputInteger("Введите количество столбцов (ключ): ");
        TableCipher cipher(key);
        cout << "Шифр инициализирован с ключом: " << key << endl;
        while (true) {
            cout << "\nВыберите операцию:" << endl;
            cout << "1 - Шифрование" << endl;
            cout << "2 - Расшифрование" << endl;
            cout << "0 - Выход" << endl;
            cout << "Ваш выбор: ";
            
            string choice_str;
            getline(cin, choice_str);
            
            if (choice_str.empty()) {
                cout << "Введите число от 0 до 2" << endl;
                continue;
            }
            
            int choice;
            try {
                choice = stoi(choice_str);
            } catch (...) {
                cout << "Неверный ввод. Введите число от 0 до 2" << endl;
                continue;
            }
            
            if (choice == 0) {
                cout << "Выход из программы." << endl;
                break;
            }
            
            try {
                if (choice == 1 || choice == 2) {
                    wstring text = inputText("Введите текст: ");
                    
                    if (choice == 1) {
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    }
                } else {
                    cout << "Неверная операция. Введите 0, 1 или 2." << endl;
                }
            } catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
        }
        
    } catch (const exception& e) {
        cerr << "Критическая ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
