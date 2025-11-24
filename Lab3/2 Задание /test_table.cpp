#include <UnitTest++/UnitTest++.h>
#include <string>
#include <locale>
#include <codecvt>
#include "table.h"

using namespace std;

string wideToUtf8(const wstring& ws) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(ws);
}

wstring utf8ToWide(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

#define CHECK_WIDE_EQUAL(expected, actual) \
    CHECK_EQUAL(wideToUtf8(expected), wideToUtf8(actual))

SUITE(ConstructorTest)
{
    TEST(ValidKey) {
        Table cipher(3);
        CHECK_WIDE_EQUAL(L"ИТРРЕИПВМ", cipher.encrypt(L"ПРИВЕТМИР"));
    }
    
    TEST(LongKey) {
        Table cipher(10);
        CHECK_WIDE_EQUAL(L"ТЕВИРП", cipher.encrypt(L"ПРИВЕТ"));
    }
    
    TEST(KeyEqualsMessageLength) {
        Table cipher(9);
        CHECK_WIDE_EQUAL(L"РИМТЕВИРП", cipher.encrypt(L"ПРИВЕТМИР"));
    }
    
    TEST(NegativeKey) {
        CHECK_THROW(Table cipher(-3), cipher_error);
    }
    
    TEST(ZeroKey) {
        CHECK_THROW(Table cipher(0), cipher_error);
    }
}

struct Key3Fixture {
    Table* cipher;
    
    Key3Fixture() {
        cipher = new Table(3);
    }
    
    ~Key3Fixture() {
        delete cipher;
    }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(Key3Fixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"ИТРРЕИПВМ", cipher->encrypt(L"ПРИВЕТМИР"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowerCaseString) {
        CHECK_WIDE_EQUAL(L"ИТРРЕИПВМ", cipher->encrypt(L"приветмир"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithWhitespace) {
        CHECK_WIDE_EQUAL(L"ИТРРЕИПВМ", cipher->encrypt(L"ПРИВЕТ МИР"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithNumbers) {
        CHECK_WIDE_EQUAL(L"ИТРЕПВ", cipher->encrypt(L"ПРИВЕТ"));
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(cipher->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoLetters) {
        CHECK_THROW(cipher->encrypt(L"1234"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithPunctuation) {
        CHECK_WIDE_EQUAL(L"ИТРРЕИПВМ", cipher->encrypt(L"ПРИВЕТ, МИР"));
    }
    
    TEST_FIXTURE(Key3Fixture, ShortString) {
        CHECK_WIDE_EQUAL(L"П", cipher->encrypt(L"П"));
    }
    
    TEST_FIXTURE(Key3Fixture, TwoCharString) {
        CHECK_WIDE_EQUAL(L"ИП", cipher->encrypt(L"ПИ"));
    }

    TEST(NonMultipleKeyLength) {
        Table cipher(5);
        CHECK_WIDE_EQUAL(L"ЕВРИИРМПТ", cipher.encrypt(L"ПРИВЕТМИР"));
    }
    
    TEST(NonMultipleKeyLength2) {
        Table cipher(11);
        CHECK_WIDE_EQUAL(L"РИМТЕВИРП", cipher.encrypt(L"ПРИВЕТМИР"));
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(Key3Fixture, UpperCaseString) {
        CHECK_WIDE_EQUAL(L"ПРИВЕТМИР", cipher->decrypt(L"ИТРРЕИПВМ"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowerCaseString) {
        CHECK_THROW(cipher->decrypt(L"итреиПВМ"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, WhitespaceString) {
        CHECK_THROW(cipher->decrypt(L"ИТР РЕИ ПВМ"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, DigitsString) {
        CHECK_THROW(cipher->decrypt(L"ИТРЕПВ2024"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(cipher->decrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoLettersDecrypt) {
        CHECK_THROW(cipher->decrypt(L"1234"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, ShortStringDecrypt) {
        CHECK_WIDE_EQUAL(L"П", cipher->decrypt(L"П"));
    }
    
    TEST_FIXTURE(Key3Fixture, TwoCharStringDecrypt) {
        CHECK_WIDE_EQUAL(L"ПИ", cipher->decrypt(L"ИП"));
    }

    TEST_FIXTURE(Key3Fixture, ValidCipherText) {
        CHECK_WIDE_EQUAL(L"ПРИВЕТМИР", cipher->decrypt(L"ИТРРЕИПВМ"));
    }

    TEST(NonMultipleKeyLengthDecrypt) {
        Table cipher(5);
        CHECK_WIDE_EQUAL(L"ПРИВЕТМИР", cipher.decrypt(L"ЕВРИИРМПТ"));
    }
    
    TEST(NonMultipleKeyLengthDecrypt2) {
        Table cipher(11);
        CHECK_WIDE_EQUAL(L"ПРИВЕТМИР", cipher.decrypt(L"РИМТЕВИРП"));
    }
}

int main(int argc, char** argv)
{
    return UnitTest::RunAllTests();
}
