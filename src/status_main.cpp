// Status program v0.0.1
// By: xabyxd
#include <iostream>
#include <unistd.h> // for sleep()
#include <termios.h> // for tcgetattr() and tcsetattr()
#include <fstream> // for ifstream
#include <iomanip> // for setprecision()
#include <chrono>
#include <thread> // for sleep_until()
#include <cstdlib> // for system()

// Define color codes for ANSI for Linux/Unix-based systems
#define RESET       "\033[0m"    // Default console color
#define BLACK       "\033[30m"   // Black
#define RED         "\033[31m"   // Red
#define GREEN       "\033[32m"   // Green
#define YELLOW      "\033[33m"   // Yellow
#define BLUE        "\033[34m"   // Blue
#define MAGENTA     "\033[35m"   // Magenta
#define CYAN        "\033[36m"   // Cyan
#define WHITE       "\033[37m"   // White
#define BOLDBLACK   "\033[1;30m" // Intense black (gray)
#define BOLDRED     "\033[1;31m" // Intense red
#define BOLDGREEN   "\033[1;32m" // Intense green
#define BOLDYELLOW  "\033[1;33m" // Intense yellow
#define BOLDBLUE    "\033[1;34m" // Intense blue
#define BOLDMAGENTA "\033[1;35m" // Intense magenta
#define BOLDCYAN    "\033[1;36m" // Intense cyan
#define BOLDWHITE   "\033[1;37m" // Intense white

// Function to set the console text color
void setColor(const char* color) {
    std::cout << color;
}

void clearTerm() { // cstdlib for clear terminal using system()
    system("clear");
}

void clearTermANSI() { // ANSI escape code
    std::cout << "\033[H\033[2J\033[3J";
    std::cout.flush();
}

void programTitle() {
    std::cout << "Status program v0.0.1 - Raspberry Pi" << std::endl;
    std::cout << "By: xabyxd" << std::endl;
    // programTitle("System Monitoring Tool"); (alternative)
}

/* New functions TODO: Implement
void printCPUInfo() {
    std::cout << "CPU: " << std::endl;
    std::cout << "\tArchitecture: " << std::endl;
    std::cout << "\t\t" << "ARMv7-A" << std::endl;
    std::cout << "\tFrequency: " << std::endl;
    std::cout << "\t\t" << "Max: " << "1.2 GHz" << std::endl;
    std::cout << "\t\t" << "Min: " << "1.2 GHz" << std::endl;
    std::cout << "\tTemperature: " << std::endl;
    std::cout << "\t\t" << "Current: " << "25.0 °C" << std::endl;
    std::cout << "\tCores: " << std::endl;
    std::cout << "\t\t" << "Total: " << "4" << std::endl;
    std::cout << "\t\t" << "Available: " << "4" << std::endl;
}
*/

float getCPUTemperature() {
    const std::string tempPath = "/sys/class/thermal/thermal_zone0/temp";
    std::ifstream tempFile(tempPath);
    if (!tempFile.is_open()) {
        setColor(RED);
        throw std::runtime_error("Can't open CPU temperature file.");
        setColor(RESET);
    }

    int tempMilliCelsius;
    tempFile >> tempMilliCelsius;
    tempFile.close();

    // Convert from millidegrees Celsius to degrees Celsius
    return tempMilliCelsius / 1000.0;
}

void printCPUTemperature() {
    try {
        float temperature = getCPUTemperature();
        std::cout << "CPU Temperature: "
                  << std::fixed << std::setprecision(2) << temperature
                  << " °C" << std::endl;
    } catch (const std::exception& e) {
        setColor(RED);
        std::cerr << "Error: " << e.what() << std::endl;
        setColor(RESET);
    }
}

// Configuration of input to disable terminal buffering
void disableCanonicMode() {
    termios terminal;
    tcgetattr(STDIN_FILENO, &terminal);
    terminal.c_lflag &= ~ICANON;  // Canonic mode
    terminal.c_lflag &= ~ECHO;    // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

// Restore terminal configuration
void restoreCanonicMode() {
    termios terminal;
    tcgetattr(STDIN_FILENO, &terminal);
    terminal.c_lflag |= ICANON;
    terminal.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

// Function to check if a key has been pressed
bool pressedKey() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

int main() {
    const int interval = 2; // seconds
    programTitle();
    std::cout << "Press 'q' to exit the program." << std::endl;
    while (true) {
        printCPUTemperature();

        for (int i = 0; i < interval; i++) {
            if (pressedKey()) {
                char c;
                read(STDIN_FILENO, &c, 1);  // Read a single character
                if (c == 'q') {
                    setColor(YELLOW);
                    std::cout << "Exiting program..." << std::endl;
                    setColor(RESET);
                    restoreCanonicMode();
                    return 0;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // Restore terminal configuration
    restoreCanonicMode();
    return 0;
}