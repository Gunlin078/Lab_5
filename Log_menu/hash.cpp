#include "hash.h"

/** (Private)
 * Генерирация SHA-256 хеша из входной строки
 * @param input - строка для хеширования
 * @return строковое представление хеша в шестнадцатеричном формате
 */
std::string Hash::generateHash(const string& input) {
    BCRYPT_ALG_HANDLE bHandle = NULL;           // Дескриптор алгоритма хеширования
    std::stringstream strStream;                // Открываю поток
    DWORD hashLen = 32;                         // Длина SHA-256 хеша (32 байта)
    BYTE hash[32];                              // Массив для хранения бинарного хеша

    // Открываем провайдер алгоритма SHA-256
    //                          дескриптор, идент-р алгоритма,       провайдер по умолчанию, стандартный режим
    BCryptOpenAlgorithmProvider(&bHandle,   BCRYPT_SHA256_ALGORITHM, NULL,                   0);
    //1. Обращается к криптографической подсистеме Windows
    //2. Загружает/активирует алгоритм SHA-256 в памяти
    //3. Создаёт дескриптор (указатель/идентификатор) этого алгоритма и сохраняет его в переменную bHandle


    // Вычисляем хеш: преобразуем входную строку в байты и получаем хеш
    //                           преобразование,       длина,          куда, размер
    BCryptHash(bHandle, NULL, 0, (PBYTE)input.c_str(), input.length(), hash, hashLen);

    // Закрываем провайдер алгоритма
    BCryptCloseAlgorithmProvider(bHandle, 0);

    // Преобразуем бинарный хеш в шестнадцатеричную строку
    for (int i = 0; i < 32; i++) {
        // Каждый байт преобразуем в 2 шестнадцатеричных символа с ведущим нулем
        strStream << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return strStream.str();  // Возвращую весь поток
}

/** (Private)
 * Генерирует криптографически безопасную случайную соль
 * @return строковое представление соли в шестнадцатеричном формате
 */
std::string Hash::generateSalt() {
    std::vector<BYTE> salt(16);  // Вектор для хранения соли (16 байт = 128 бит)
    std::stringstream strStream;   // Открываю поток

    // Генерируем криптографически стойкие случайные байты
    BCryptGenRandom(NULL, salt.data(), salt.size(), BCRYPT_USE_SYSTEM_PREFERRED_RNG);

    // Преобразуем бинарную соль в шестнадцатеричную строку
    for (BYTE b : salt) {
        strStream << std::hex << std::setw(2) << std::setfill('0') << (int)b;
    }
    return strStream.str();  // Возвращаем строку с солью
}

/** (Public)
 * Хеширует строку с использованием соли и фиксированной соли
 * @param: input - строка для хеширования
 * @param: fixedSalt - фиксированная соль для дополнительной защиты
 * @return: строка в формате "сгенерированная_соль:хеш"
 */
std::string Hash::hashString_s(const string& input, const string& fixedSalt) {
    std::string salt = generateSalt();  // Генерируем уникальную соль
    // Комбинируем входные данные: оригинал + соль + фиксированная соль
    std::string saltedInput = input + salt + fixedSalt;
    std::string hash = generateHash(saltedInput);  // Хешируем комбинацию

    // Возвращаем соль и хеш через разделитель для последующей проверки
    return salt + ":" + hash;
}

/** (Public)
 * Проверяет соответствие исходной строки сохраненному хешу с солью
 * @param input_pass - исходная строка для проверки
 * @param saltedHash - сохраненная строка в формате "соль:хеш"
 * @param fixedSalt - фиксированная соль, использованная при хешировании
 * @return 1 - если строка соответствует хешу, 0 - если не соответствует, -1 при ошибке формата
 */
int Hash::verifyString_s(const string& input_pass, const string& saltedHash, const string& fixedSalt) {
    // Ищем разделитель между солью и хешем
    size_t sepPos = saltedHash.find(':');
    if (sepPos == saltedHash.npos) return -1;  // Ошибка: неверный формат строки

    // Извлекаем соль и хеш из сохраненной строки
    std::string salt = saltedHash.substr(0, sepPos);
    std::string hash = saltedHash.substr(sepPos + 1);

    // Формируем ту же комбинацию, что и при хешировании
    std::string saltedinput_pass = input_pass + salt + fixedSalt;
    // Вычисляем хеш для проверяемой строки
    std::string newHash = generateHash(saltedinput_pass);

    if (newHash == hash) {return 1;}  // Строки совпадают
    return 0;  // Строки не совпадают
}
