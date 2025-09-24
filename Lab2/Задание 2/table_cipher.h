#ifndef TABLE_CIPHER_H
#define TABLE_CIPHER_H

#include <string>
#include <stdexcept>
using namespace std;

class TableCipher
{
private:
    int columns;
    
public:
    TableCipher(int key);
    wstring encrypt(const wstring& plain_text);
    wstring decrypt(const wstring& cipher_text);
};

#endif
