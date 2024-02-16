#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

void IncrementCounterInFile(const std::string& filePath) {
    // �}�Ҥ���Ū���p�ƾ���
    std::ifstream fileRead(filePath);
    int counter = 0;
    if (fileRead.is_open()) {
        fileRead >> counter;
        fileRead.close();
    }

    // �W�[�p�ƾ���
    counter++;

    // �g�^�s���p�ƾ��Ȩ���
    std::ofstream fileWrite(filePath);
    if (fileWrite.is_open()) {
        fileWrite << counter;
        fileWrite.close();
    }
}

int main() {
    // ���ե��}�γЫؤ@�� Mutex
    HANDLE hMutex = CreateMutex(NULL, FALSE, L"MySharedMutex");
    if (hMutex == NULL) {
        std::cerr << "CreateMutex error: " << GetLastError() << std::endl;
        return 1;
    }

    // ������o Mutex ���֦��v
    WaitForSingleObject(hMutex, INFINITE);

    // ��s�@�ɤ��
    IncrementCounterInFile("shared_counter.txt");

    // ���� Mutex
    ReleaseMutex(hMutex);

    // ���� Mutex ��H�y�`
    CloseHandle(hMutex);
    return 0;
}

