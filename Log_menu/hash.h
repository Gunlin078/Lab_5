#ifndef HASH_H
#define HASH_H
#define FIXED_SALT "MyFixedSalt123!@#"  //фиксированная соль
#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
using namespace std;
class Hash
{
public:
    Hash();
    static string hashString_s(const string& input, const string& fixedSalt);  // Хеширование строки с использованием персональной и общей соли через generateHash
    static int verifyString_s(const string& base, const string& saltedHash, const string& fixedSalt); //Проверяет соответствие исходной строки сохраненному хешу с солью
private:
    static string generateSalt();                                              // Генерирует уникальную соль
    static string generateHash(const string& input);                           // Генерирует хэш из строки
};
#endif
