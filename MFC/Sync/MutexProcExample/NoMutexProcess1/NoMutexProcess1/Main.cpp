#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void WriteToFile(const std::string& filename, const std::string& processInfo, int updateCount) 
{
    std::ofstream file;
    file.open(filename, std::ios_base::app); // �}�Ҥ��
    if (file.is_open()) 
    {
        file << processInfo << "�A��s����: " << updateCount << std::endl;
        file.close();
    }
    else 
    {
        std::cerr << "�L�k�}�Ҥ��i��g�J�C" << std::endl;
    }
}

int main() 
{
    const std::string filename = "NoMutexExample.txt";  // �g�J�����W��
    const int totalSeconds = 10; // �`�@��s10��

    for (int i = 1; i <= totalSeconds; ++i) 
    {
        // �C���`���ͦ����T���N�]�A��{ID�B���W�M��e��s����
        std::string processInfo = "�Ѧ�{ID�ק�: " + std::to_string(GetCurrentProcessId()) + ", ���: P1.cpp";
        WriteToFile(filename, processInfo, i);

        // �Ȱ��@��A�M��i��U�@����s
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
