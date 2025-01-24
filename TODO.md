// TODO: Implement

[ ✔ ] Base program structure and main functions
[ ❌ ] More system data/information
[ ❌ ] RAM usage
[ ❌ ] Output format
[ ❌ ] Abstract class for CPU temperature, frequency, usage, cores


int main() {
    try {
        disableCanonicMode();
        // Resto del código...
    } catch (const std::exception& e) {
        setColor(RED);
        std::cerr << "Error fatal: " << e.what() << std::endl;
        setColor(RESET);
        restoreCanonicMode();
        return 1;
    }
}


Output abstraction

void printRow(const std::string& label, const std::string& value, const char* color = RESET) {
    setColor(color);
    std::cout << label << ": " << value << RESET << std::endl;
}

More system data/information

float getCPUUsage() {
    std::ifstream procStat("/proc/stat");
    if (!procStat) throw std::runtime_error("No se pudo leer /proc/stat");
    
    std::string line;
    std::getline(procStat, line); // Leer la primera línea (cpu)
    procStat.close();

    std::istringstream iss(line);
    std::string cpu;
    long user, nice, system, idle;
    iss >> cpu >> user >> nice >> system >> idle;
    
    static long prevIdle = 0, prevTotal = 0;
    long total = user + nice + system + idle;
    long diffTotal = total - prevTotal;
    long diffIdle = idle - prevIdle;

    prevTotal = total;
    prevIdle = idle;

    return 100.0 * (diffTotal - diffIdle) / diffTotal;
}

RAM usage

std::pair<int, int> getRAMUsage() {
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo) throw std::runtime_error("No se pudo leer /proc/meminfo");

    std::string key;
    int value, totalRAM = 0, availableRAM = 0;
    while (meminfo >> key >> value) {
        if (key == "MemTotal:") totalRAM = value;
        if (key == "MemAvailable:") availableRAM = value;
        if (totalRAM > 0 && availableRAM > 0) break;
    }
    meminfo.close();

    return {totalRAM, totalRAM - availableRAM}; // {Total, Usada}
}


Output format

CPU Temperature: 42.00 °C
CPU Usage:       12.30 %
RAM Usage:       1200 MB / 4000 MB
