#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>

using namespace std;

class modAlphaCipher
{
private:
    const wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; 
    map<wchar_t, int> alphaNum; 
    vector<int> key; 
    
    vector<int> convert(const wstring& text);
    wstring convert(const vector<int>& numbers);

public:
    modAlphaCipher() = delete; 
    modAlphaCipher(const wstring& skey);
    wstring encrypt(const wstring& open_text);
    wstring decrypt(const wstring& cipher_text);
};

#endif 
