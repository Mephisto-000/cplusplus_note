#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

void IncrementCounterInFile(const std::string& filePath) {
    // 開啟文件並讀取計數器值
    std::ifstream fileRead(filePath);
    int counter = 0;
    if (fileRead.is_open()) {
        fileRead >> counter;
        fileRead.close();
    }

    // 增加計數器值
    counter++;

    // 寫回新的計數器值到文件
    std::ofstream fileWrite(filePath);
    if (fileWrite.is_open()) {
        fileWrite << counter;
        fileWrite.close();
    }
}

int main() {
    // 嘗試打開或創建一個 Mutex
    HANDLE hMutex = CreateMutex(NULL, FALSE, L"MySharedMutex");
    if (hMutex == NULL) {
        std::cerr << "CreateMutex error: " << GetLastError() << std::endl;
        return 1;
    }

    // 等待獲得 Mutex 的擁有權
    WaitForSingleObject(hMutex, INFINITE);

    // 更新共享文件
    IncrementCounterInFile("shared_counter.txt");

    // 釋放 Mutex
    ReleaseMutex(hMutex);

    // 釋放 Mutex 對象句柄
    CloseHandle(hMutex);
    return 0;
}

