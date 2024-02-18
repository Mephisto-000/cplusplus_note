#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

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
    const std::string filename = "example.txt";          // 寫入的文件名稱
    const std::string mutexName = "Global\\MyAppMutex";  // Mutex 名稱

    // 創建或打開命名的互斥體
    HANDLE hMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
    if (hMutex == NULL) 
    {
        std::cerr << "CreateMutex 錯誤: " << GetLastError() << std::endl;
        return 1;
    }

    // 在訪問文件之前嘗試獲取互斥體
    DWORD dwWaitResult = WaitForSingleObject(hMutex, INFINITE); // 無限等待
    try 
    {
        if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) 
        {
            for (int i = 1; i <= 10; ++i) 
            { // 總共更新10次
                std::string processInfo = "由行程ID修改: " + std::to_string(GetCurrentProcessId()) + ", 文件: P1.cpp";
                WriteToFile(filename, processInfo, i);
                Sleep(1000); // 暫停一秒
            }
            // 完成後釋放互斥體
            if (!ReleaseMutex(hMutex)) 
            {
                std::cerr << "ReleaseMutex 錯誤: " << GetLastError() << std::endl;
            }
        }
        else 
        {
            std::cerr << "等待互斥體失敗。" << std::endl;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "發生異常: " << e.what() << std::endl;
        // 即使發生異常，也要確保釋放互斥體
        ReleaseMutex(hMutex);
    }

    CloseHandle(hMutex);
    return 0;
}
