#include "modAlphaCipher.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <locale>
#include <codecvt>

using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{

    for (size_t i = 0; i < numAlpha.size(); ++i) {
        alphaNum[numAlpha[i]] = i;
    }
    
    if (skey.empty()) {
        throw invalid_argument("Ключ не может быть пустым");
    }
    
    key = convert(skey);
}

vector<int> modAlphaCipher::convert(const wstring& text)
{
    vector<int> result;
    result.reserve(text.size()); 
    
    for (wchar_t c : text) {
        auto it = alphaNum.find(c);
        if (it == alphaNum.end()) {
            throw invalid_argument("Обнаружен недопустимый символ в тексте");
        }
        result.push_back(it->second);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& numbers)
{
    wstring result;
    result.reserve(numbers.size()); 
    
    for (int num : numbers) {
        if (num < 0 || num >= static_cast<int>(numAlpha.size())) {
            throw out_of_range("Недопустимое числовое значение символа");
        }
        result.push_back(numAlpha[num]);
    }
    return result;
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    if (open_text.empty()) {
        throw invalid_argument("Текст для шифрования не может быть пустым");
    }
    
    vector<int> work = convert(open_text);
    size_t key_size = key.size();
    size_t alpha_size = numAlpha.size();
    
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + key[i % key_size]) % alpha_size;
    }
    
    return convert(work);
}

// Расшифрование текста
wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    if (cipher_text.empty()) {
        throw invalid_argument("Текст для расшифрования не может быть пустым");
    }
    
    vector<int> work = convert(cipher_text);
    size_t key_size = key.size();
    size_t alpha_size = numAlpha.size();
    
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + alpha_size - key[i % key_size]) % alpha_size;
    }
    
    return convert(work);
}
