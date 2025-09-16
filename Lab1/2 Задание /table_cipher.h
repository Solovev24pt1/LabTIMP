#include <string>
#include <vector>
#include <stdexcept>

class TableCipher
{
private:
    int columns;
public:
    TableCipher(int key);
    std::wstring encrypt(const std::wstring& plain_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};
