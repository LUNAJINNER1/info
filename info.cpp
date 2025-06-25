#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

using namespace std;
string readProcFile(const string& path) {
    ifstream file(path);
    return file.good() ? string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()) : "N/A";
}

string getCpuInfo() {
    string info;
    ifstream cpuinfo("/proc/cpuinfo");
    string line;
    
 while (getline(cpuinfo, line)) {
     if (line.find("model name") != string::npos || 
           line.find("Hardware") != string::npos ||
           line.find("Processor") != string::npos) {
           info += line + "\n";
        }
    }
    return info.empty() ? readProcFile("/proc/cpuinfo") : info;
}

void getMemoryInfo() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        cout << "Общая RAM: " << (info.totalram * info.mem_unit / 1024 / 1024) << " MB\n";
        cout << "Свободно RAM: " << (info.freeram * info.mem_unit / 1024 / 1024) << " MB\n";
        cout << "Swap: " << (info.totalswap * info.mem_unit / 1024 / 1024) << " MB\n";
    }
}

string getBatteryInfo() {
    string capacity = readProcFile("/sys/class/power_supply/battery/capacity");
    string status = readProcFile("/sys/class/power_supply/battery/status");
    return "Заряд: " + capacity + "% (" + status + ")";
}

int main() {
    struct utsname sysinfo;
    uname(&sysinfo);

    cout << "=== Системная информация ===" << endl;
    cout << "ОС: " << sysinfo.sysname << endl;
    cout << "Версия ядра: " << sysinfo.release << endl;
    cout << "Архитектура: " << sysinfo.machine << endl;
    
    cout << "\n=== Процессор ===" << endl;
    cout << getCpuInfo();
    
    cout << "\n=== Память ===" << endl;
    getMemoryInfo();
    
    cout << "\n=== Хранилище ===" << endl;
    system("df -h | grep -E 'Filesystem|/data'");
    
    cout << "\n=== Батарея ===" << endl;
    cout << getBatteryInfo() << endl;
    
    cout << "\n=== Сеть ===" << endl;
    system("ip a show wlan0 | grep 'inet '");
    
    cout << "\n=== Графика ===" << endl;
    system("dumpsys SurfaceFlinger | grep 'GLES'");
    
    cout << "\n=== Датчики ===" << endl;
    system("dumpsys sensorservice | grep -A 5 'Active sensors:'");
    
    cout << "\n=== Установленные пакеты Termux ===" << endl;
    system("pkg list-installed");

    return 0;
}   

