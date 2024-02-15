#include <windows.h>
#include <iostream>

// �@�ɸ귽
int sharedData = 0;

// �u�{���
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    for (int i = 0; i < 100000; ++i) {
        // �o�̦s�b�v������A�]���h�ӽu�{�չϦP�ɭק�sharedData
        sharedData++;
    }
    return 0;
}

int main() {
    // �Ыب�ӽu�{
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);

    // ���ݽu�{����
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // ��X�̲ת��@�ɸ귽��
    std::cout << "Expected value: 200000" << std::endl;
    std::cout << "Actual value: " << sharedData << std::endl;

    // �����u�{�y�`
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    return 0;
}