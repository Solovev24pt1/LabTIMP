#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include "modAlphaCipher.h"
#include <limits>
using namespace std;

bool isValidRussianText(const wstring& text)
{
    static const wstring russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    
    if (text.empty()) {
        return false;
    }
    
    for (wchar_t c : text) {
        if (russianAlphabet.find(c) == wstring::npos) {
            return false;
        }
    }
    return true;
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

wstring stringToWstring(const string& str)
{
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    } catch (const exception& e) {
        cerr << "Ошибка конвертации строки: " << e.what() << endl;
        return L"";
    }
}

string wstringToString(const wstring& wstr)
{
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    } catch (const exception& e) {
        cerr << "Ошибка конвертации строки: " << e.what() << endl;
        return "";
    }
}

void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

unsigned getUnsignedInput(const string& prompt)
{
    unsigned value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            clearInputBuffer();
            cout << "Ошибка: введите число!" << endl;
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << "=== Шифр для русского алфавита ===" << endl;
    
    string key_input;
    cout << "Введите ключ (только русские буквы): ";
    getline(cin, key_input);
    
    if (key_input.empty()) {
        cerr << "Ошибка: ключ не может быть пустым!" << endl;
        return 1;
    }
    
    wstring key = toUpperRussian(stringToWstring(key_input));
    
    if (!isValidRussianText(key)) {
        cerr << "Ошибка: ключ содержит недопустимые символы!" << endl;
        cerr << "Используйте только русские буквы." << endl;
        return 1;
    }
    
    try {
        modAlphaCipher cipher(key);
        cout << "Ключ успешно загружен. Длина ключа: " << key.length() << " символов" << endl;
        
        unsigned operation;
        do {
            cout << "\nВыберите операцию:" << endl;
            cout << "1 - Шифрование текста" << endl;
            cout << "2 - Расшифрование текста" << endl;
            cout << "0 - Выход из программы" << endl;
            cout << "Ваш выбор: ";
            
            cin >> operation;
            if (cin.fail()) {
                cin.clear();
                clearInputBuffer();
                cout << "Ошибка: введите число от 0 до 2!" << endl;
                continue;
            }
            clearInputBuffer();
            
            switch (operation) {
                case 0:
                    cout << "Выход из программы. До свидания!" << endl;
                    break;
                    
                case 1:
                case 2: {
                    cout << "Введите текст: ";
                    string text_input;
                    getline(cin, text_input);
                    
                    if (text_input.empty()) {
                        cout << "Ошибка: текст не может быть пустым!" << endl;
                        break;
                    }
                    
                    wstring text = toUpperRussian(stringToWstring(text_input));
                    
                    if (!isValidRussianText(text)) {
                        cout << "Ошибка: текст содержит недопустимые символы!" << endl;
                        cout << "Используйте только русские буквы." << endl;
                        break;
                    }
                    
                    try {
                        if (operation == 1) {
                            wstring encrypted = cipher.encrypt(text);
                            cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                        } else {
                            wstring decrypted = cipher.decrypt(text);
                            cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                        }
                    } catch (const exception& e) {
                        cerr << "Ошибка при обработке текста: " << e.what() << endl;
                    }
                    break;
                }
                    
                default:
                    cout << "Ошибка: неверная операция! Выберите 0, 1 или 2." << endl;
                    break;
            }
        } while (operation != 0);
        
    } catch (const exception& e) {
        cerr << "Критическая ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
