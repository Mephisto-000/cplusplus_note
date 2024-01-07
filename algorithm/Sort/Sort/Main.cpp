#include <iostream>
#include "Sort.h"
using namespace std;



int main()
{

	const int iTestArrayLen{ 10 };
	int ayTestData[iTestArrayLen]{ 35, 52, 68, 12, 47, 52, 36, 52, 74, 27 };

	cout << "Original Data : " << endl;
	for (int iNum : ayTestData)
	{
		cout << iNum << " ";
	}
	cout << endl;
	//InsertionSort(ayTestData, iTestArrayLen);
	MergeSort(ayTestData, 0, 9);


	cout << "Sorted Data : " << endl;
	for (int iNum : ayTestData)
	{
		cout << iNum << " ";
	}
	

	cout << endl;
	return 0;
}