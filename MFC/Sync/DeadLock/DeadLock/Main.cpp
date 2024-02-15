#include <windows.h>
#include <iostream>

// �w�q��ӥ���������
HANDLE Mutex1;
HANDLE Mutex2;

// �u�{���1
DWORD WINAPI ThreadFunc1(LPVOID lpParam) {
    WaitForSingleObject(Mutex1, INFINITE); // �������Mutex1
    Sleep(100); // ���ݤ@�q�ɶ��A�W�[����o�ͪ��i���
    WaitForSingleObject(Mutex2, INFINITE); // �M��չ����Mutex2

    std::cout << "Thread 1 has both Mutex1 and Mutex2" << std::endl;

    ReleaseMutex(Mutex2);
    ReleaseMutex(Mutex1);

    return 0;
}

// �u�{���2
DWORD WINAPI ThreadFunc2(LPVOID lpParam) {
    WaitForSingleObject(Mutex2, INFINITE); // �������Mutex2
    Sleep(100); // ���ݤ@�q�ɶ��A�W�[����o�ͪ��i���
    WaitForSingleObject(Mutex1, INFINITE); // �M��չ����Mutex1

    std::cout << "Thread 2 has both Mutex1 and Mutex2" << std::endl;

    ReleaseMutex(Mutex1);
    ReleaseMutex(Mutex2);

    return 0;
}

int main() {
    // �Ыب�Ӥ�����
    Mutex1 = CreateMutex(NULL, FALSE, NULL);
    Mutex2 = CreateMutex(NULL, FALSE, NULL);

    // �Ыب�ӽu�{
    HANDLE Thread1 = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, NULL);
    HANDLE Thread2 = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, NULL);

    // ���ݽu�{����
    WaitForSingleObject(Thread1, INFINITE);
    WaitForSingleObject(Thread2, INFINITE);

    // �����u�{�M������y�`
    CloseHandle(Thread1);
    CloseHandle(Thread2);
    CloseHandle(Mutex1);
    CloseHandle(Mutex2);

    return 0;
}
