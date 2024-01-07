#include "Sort.h"



/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: InsertionSort

  Summary:  插入排序法

  Args:     
	int ayData[]
		未排序陣列。
	int iLen
		未排序陣列長度。

  Returns:	None.
-----------------------------------------------------------------F-F*/
void InsertionSort (int ayData[], int iLen)
{
	for (int i = 1; i < iLen; i++)
	{
		int iValue = ayData[i];

		int j = i - 1;

		while (iValue < ayData[j] && j >= 0)
		{
			ayData[j + 1] = ayData[j];
			j--;
		}

		ayData[j + 1] = iValue;

	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: ShellSort

  Summary:  謝爾排序法

  Args:
	int ayData[]
		未排序陣列。
	int iLen
		未排序陣列長度。

  Returns:	None.
-----------------------------------------------------------------F-F*/
void ShellSort (int ayData[], int iLen)
{
	for (int iGap = iLen / 2; iGap > 0; iGap /= 2)
	{
		for (int i = iGap; i < iLen; i += 1)
		{
			int iValue = ayData[i];
			int j;
			for (j = i; j >= iGap && ayData[j - iGap] > iValue; j -= iGap)
			{
				ayData[j] = ayData[j - iGap];
			}

			ayData[j] = iValue;

		}
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: SelectionSort

  Summary:  選擇排序法

  Args:
	int ayData[]
		未排序陣列。
	int iLen
		未排序陣列長度。

  Returns:	None.
-----------------------------------------------------------------F-F*/
void SelectionSort (int ayData[], int iLen)
{
	for (int i = 0; i < iLen - 1; i++)
	{
		int iMinimalIndex = i;
		for (int j = i + 1; j < iLen; j++)
		{
			if (ayData[j] < ayData[iMinimalIndex])
			{
				iMinimalIndex = j;
			}
		}

		int iTmp = ayData[iMinimalIndex];
		ayData[iMinimalIndex] = ayData[i];
		ayData[i] = iTmp;

	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: BubbleSort

  Summary:  泡泡排序法

  Args:
	int ayData[]
		未排序陣列。
	int iLen
		未排序陣列長度。

  Returns:	None.
-----------------------------------------------------------------F-F*/
void BubbleSort (int ayData[], int iLen)
{
	for (int i = 0; i < iLen - 1; i++)
	{
		for (int j = 0; j < iLen - i - 1; j++)
		{
			if (ayData[j] > ayData[j + 1])
			{
				int iTmp = ayData[j];
				ayData[j] = ayData[j + 1];
				ayData[j + 1] = iTmp;
			}
		}
	}
}

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: ShakerSort

  Summary:  雙向冒泡排序法

  Args:
	int ayData[]
		未排序陣列。
	int iLen
		未排序陣列長度。

  Returns:	None.
-----------------------------------------------------------------F-F*/
void ShakerSort(int ayData[], int iLen)
{
	int iLeft = 0;
	int iRight = iLen - 1;
	int iShift = 1;

	while (iLeft < iRight)
	{
		for (int i = iLeft; i < iRight; i++)
		{
			if (ayData[i] > ayData[i + 1])
			{
				int iTmp = ayData[i];
				ayData[i] = ayData[i + 1];
				ayData[i + 1] = iTmp;

				iShift = i;
			}
		}

		iRight = iShift;

		for (int i = iRight - 1; i >= iLeft; i--)
		{
			if (ayData[i] > ayData[i + 1])
			{
				int iTmp = ayData[i];
				ayData[i] = ayData[i + 1];
				ayData[i + 1] = iTmp;

				iShift = i + 1;
			}
		}

		iLeft = iShift;

	}
}