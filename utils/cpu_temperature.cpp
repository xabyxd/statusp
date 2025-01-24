#include <iostream>
#include <fstream>
#include <iomanip>

float getCPUTemperature() {
    const std::string tempPath = "/sys/class/thermal/thermal_zone0/temp";
    std::ifstream tempFile(tempPath);
    if (!tempFile.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de temperatura del CPU.");
    }

    int tempMilliCelsius;
    tempFile >> tempMilliCelsius;
    tempFile.close();

    // Convertir de miligrados Celsius a grados Celsius
    return tempMilliCelsius / 1000.0;
}

int main() {
    try {
        float temperature = getCPUTemperature();
        std::cout << "Temperatura actual del CPU: " 
                  << std::fixed << std::setprecision(2) << temperature 
                  << " °C" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
