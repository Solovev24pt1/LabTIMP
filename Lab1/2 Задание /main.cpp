#include <iostream>
#include <locale>
#include <codecvt>
#include "table_cipher.h"
using namespace std;

bool isValid(const wstring& s)
{
    wstring russianAlphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto c : s) {
        if (russianAlphabet.find(c) == wstring::npos) {
            return false;
        }
    }
    return true;
}
wstring toUpperRussian(const wstring& s)
{
    wstring result = s;
    wstring lower = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring upper = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (auto& c : result) {
        size_t pos = lower.find(c);
        if (pos != wstring::npos) {
            c = upper[pos];
        }
    }
    return result;
}

wstring stringToWstring(const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstringToString(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string key_input;
    string text_input;
    unsigned op;
    cout << "=== Шифр табличной перестановки ===" << endl;
    cout << "Шифр готов. Введите количество столбцов: ";
    getline(cin, key_input);
    try {
        int key = stoi(key_input);
        if (key <= 0) {
            cerr << "Ключ недействителен!" << endl;
            return 1;
        }
        cout << "Ключ загружен" << endl;
        TableCipher cipher(key);
        do {
            cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            cin >> op;
            cin.ignore();
            
            if (op > 2) {
                cout << "Неверная операция" << endl;
            } else if (op > 0) {
                cout << "Введите текст: ";
                getline(cin, text_input);
                wstring text = stringToWstring(text_input);
                text = toUpperRussian(text);
                
                if (isValid(text)) {
                    if (op == 1) {
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    }
                } else {
                    cout << "Используйте только русские буквы." << endl;
                }
            }
        } while (op != 0);
        
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    return 0;
}
