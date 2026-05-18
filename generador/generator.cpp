#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distID(1, 99999);

    while (true) {
        int numero = distID(rng);

        std::ostringstream oss;
        oss << "PX-" << std::setw(5) << std::setfill('0') << numero;
        std::string id = oss.str();

        std::time_t t = std::time(nullptr);
        std::tm* tm_info = std::localtime(&t);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", tm_info);
        std::string timestamp = buffer;

        std::string mensaje = id + "|" + timestamp;

        std::cout << "Enviando: " << mensaje << "\n";
    }
    return 0;
}