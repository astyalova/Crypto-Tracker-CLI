#include "crypto.h"

int main() {
    std::string value;

    std::cout << "Введите интересующую крипту(bitcoin, ethereum, dogecoin или другую):\n";
    std::cin >> value;

    CryptoInfo crypto(value);
    crypto.printInfoWithUpd();

    return 0;
}