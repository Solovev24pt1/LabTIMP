/**
 * @file main.cpp
 * @author Соловьев Арсений Евгеньевич
 * @version 1.0
 * @date 2025
 * @brief Главный модуль программы для шифрования/расшифрования текста
 * @details Реализует пользовательский интерфейс для работы с шифром Гронсфельда
 */

#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>
#include <limits>
#include "modAlphaCipher.h"

using namespace std;

/**
 * @brief Преобразование строки UTF-8 в широкую строку
 * @param s строка в UTF-8
 * @return широкая строка
 */
 
wstring str8_to_w(const string& s)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

/**
 * @brief Преобразование широкой строки в строку UTF-8
 * @param ws широкая строка
 * @return строка в UTF-8
 */
 
string w_to_str8(const wstring& ws)
{
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

/**
 * @brief Главная функция программы
 * @return код завершения программы
 * @details Реализует диалоговый интерфейс для шифрования/расшифрования
 */
 
int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string keyLine;
    string msgLine;
    unsigned action;

    cout << "Введите ключ: ";
    getline(cin, keyLine);

    try {
        modAlphaCipher cipher(str8_to_w(keyLine));
        cout << "Ключ загружен." << endl;

        do {
            cout << "Выберите режим (0 — выход, 1 — шифрование, 2 — расшифровка): ";
            if (!(cin >> action))
                return 0;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (action > 2) {
                cout << "Неверный выбор режима." << endl;
            } else if (action > 0) {
                cout << "Введите строку: ";
                getline(cin, msgLine);

                try {
                    if (action == 1) {
                        wstring enc = cipher.encrypt(str8_to_w(msgLine));
                        cout << "Зашифровано: " << w_to_str8(enc) << endl;
                    } else {
                        wstring dec = cipher.decrypt(str8_to_w(msgLine));
                        cout << "Расшифровано: " << w_to_str8(dec) << endl;
                    }
                } catch (const cipher_error& e) {
                    cerr << "Ошибка при обработке текста: " << e.what() << endl;
                }
            }
        } while (action != 0);
        
    } catch (const cipher_error& e) {
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        return 1;
    }

    return 0;
}
