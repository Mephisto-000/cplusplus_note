#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void WriteToFile(const std::string& filename, const std::string& processInfo, int updateCount) 
{
    std::ofstream file;
    file.open(filename, std::ios_base::app); // 開啟文件
    if (file.is_open()) 
    {
        file << processInfo << "，更新次數: " << updateCount << std::endl;
        file.close();
    }
    else 
    {
        std::cerr << "無法開啟文件進行寫入。" << std::endl;
    }
}

int main() 
{
    const std::string filename = "NoMutexExample.txt";  // 寫入的文件名稱
    const int totalSeconds = 10; // 總共更新10秒

    for (int i = 1; i <= totalSeconds; ++i) 
    {
        // 每次循環生成的訊息將包括行程ID、文件名和當前更新次數
        std::string processInfo = "由行程ID修改: " + std::to_string(GetCurrentProcessId()) + ", 文件: P1.cpp";
        WriteToFile(filename, processInfo, i);

        // 暫停一秒，然後進行下一次更新
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
