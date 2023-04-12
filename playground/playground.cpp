#include <iostream>
#include <chrono>
#include <ctime>

int main() {
    auto start = std::chrono::system_clock::now();
    auto legacyStart = std::chrono::system_clock::to_time_t(start);
    char tmBuff[30];
    ctime_s(tmBuff, sizeof(tmBuff), &legacyStart);
    std::cout << tmBuff << '\n';
}